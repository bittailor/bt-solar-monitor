var os = require('os');
var path = require('path');
var globby = require('globby')
var spawn = require('child_process').spawn;
var fs = require('fs-extra')
var del = require('del');
var gulp = require('gulp');
var plugins = require('gulp-load-plugins')();
var Q = require('q');
var Particle = require('particle-api-js');
var particle = new Particle();

var pwd = process.cwd();

var conf = {
    device: "bt-electron-solar",
    platform: "electron",
    //device: "bt-photon-2",
    //platform: "photon",
    fw: {
        out: 'fw/target',
        ext: 'fw/external'
    },
    web: {
        api: 'web-api/solar-api'
    }
};

//----

function getParticleAccessToken() {
    var json = JSON.parse(fs.readFileSync(path.join(os.homedir(),'.particle/particle.config.json')));
    return json.access_token;
}

//----

function sh(cmd, args, cwd, stdio = 'inherit' ) {
    var deferred = Q.defer();
    plugins.util.log(plugins.util.colors.gray('$', cmd , args.join(" ")));
    var options = {
        stdio: stdio,
        cwd: cwd,
    }
    spawn(cmd, args, options).on('exit', (code) => {
        if(code !== 0) {
            deferred.reject(code);
        } else {
            deferred.resolve(code);
        }     
    });
    return deferred.promise;
}

//----

function installGit(url , branch) {
    var deferred = Q.defer();
    plugins.util.log(`** install git ${url} ${branch} **`);    
    var args = ['clone'];
    if (branch) {
        args.push('--branch');
        args.push(branch);    
    }
    args.push(url);    
    return sh('git', args, conf.fw.ext)
}

//----

//----

function deleteSolarWebHooks() {
    return particle
        .listWebhooks({auth: getParticleAccessToken()})
        .then((data) => {
            var hooks = data.body;
            solarHooks = hooks.filter(hook => hook.event.startsWith('solar/'));
            plugins.util.log(`Found ${solarHooks.length} solar hooks`);
            deletes = solarHooks.map((hook) => {
                return particle.deleteWebhook({
                    auth: getParticleAccessToken(),
                    hookId: hook.id   
                })
                .then((data) => {
                    plugins.util.log("delete : ",data);
                });
            });
            return Promise.all(deletes);
        });   
}

function createSolarWebHooks() {
    return globby(['web-hooks/**/*.json', '!web-hooks/**/*.mustache.json'])
        .then((files)=>{
            //plugins.util.log('hook files',files); 
            creates = files.map((file)=>{
                return sh('particle',['webhook', 'create', file], null, [process.stdin, 'ignore', process.stderr]);
            });
            return Promise.all(creates);
        })
        ; 
}

//----

gulp.task('web::hooks::deleteAll', () => {
    return deleteSolarWebHooks();
});

gulp.task('web::hooks::createAll',['web::hooks::deleteAll','bs:secrets'] , () => {
    return createSolarWebHooks();
});

gulp.task('web::hooks',['web::hooks::createAll']);

//----

gulp.task('install-ext', function(){    
    return fs.mkdirs(conf.fw.ext).then(function(){
        var clones = [];
        clones.push(installGit('https://github.com/google/googletest.git', 'release-1.8.0'));
        return Q.all(clones);
    });  
});

gulp.task('install',['install-ext']);


//----

gulp.task('bs:secrets',() => {
    return gulp
        .src("./**/*.mustache.*")
        .pipe(plugins.mustache('./secrets.json',{tags: ['{${', '}$}']}))
        .pipe(plugins.debug({title: 'mustache-in:'}))
        .pipe(plugins.simpleRename((path) => {
            return path.replace(/\.mustache\./, '.generated.');
        }))
        .pipe(plugins.debug({title: 'mustache-out:'}))
        .pipe(gulp.dest("."))
        ;    
});

//----

gulp.task('fw:host:test', () => {
    var out = path.join(conf.fw.out, 'host-test');
    return fs.mkdirs(out)
    .then(() => {       
        return sh('cmake', ['-DCMAKE_BUILD_TYPE=Debug','../../'], out);
    })
    .then(()=>{
        return sh('make', ['check'], out);
    });
});

//----

gulp.task('web:api:test', () => {
    var cwd = conf.web.api;
    return sh('vendor/bin/phpunit', ['tests'], cwd);
});

gulp.task('web:api:serve', () => {
    var cwd = path.join(conf.web.api,'public');
    return sh('php', ['-S', 'localhost:8080'], cwd);
});

//----

gulp.task('clean-make', function(cb) {
    spawn('make', ['clean', 'PLATFORM=' + conf.platform, 'APPDIR=' + path.join(pwd, 'fw')],{
        stdio: 'inherit',
        cwd:'../firmware/main'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('clean-dir', function () {
    return del([
        conf.fw.out
    ]);
});

gulp.task('clean',['clean-dir','clean-make'])

gulp.task('compile-core', function(cb){
    spawn('make', ['all', 'PLATFORM=' + conf.platform, 'APPDIR=' + path.join(pwd, 'fw')],{
        stdio: 'inherit',
        cwd:'../firmware/modules'
    }).on('exit', (code) => {
        cb(code)
    }); 

});

gulp.task('compile-local', function(cb){
    spawn('make' , [ /*'-s',*/ 'all', 'PLATFORM=' + conf.platform, 'APPDIR=' + path.join(pwd, 'fw')],{
        stdio: 'inherit',
        cwd:'../firmware/main'
    }).on('exit', (code) => {
        cb(code)
    }); 

});

gulp.task('compile-online', function(cb){
    fs.mkdirs(conf.fw.out,function(err){
        spawn('particle', ['compile', conf.platform, '.' ,  '--saveTo' ,  path.join('target' , 'fw.bin')],{
            stdio: 'inherit',
            cwd:'./fw'
        }).on('exit', (code) => {
            cb(code)
        }); 
    })
});

gulp.task('flash-local', function(cb){
    spawn('particle', ['flash', conf.device, './target/fw.bin'],{
        stdio: 'inherit',
        cwd:'./fw'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('flash-usb', function(cb){
    spawn('particle', ['flash', '--usb', './target/fw.bin'],{
        stdio: 'inherit',
        cwd:'./fw'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('flash-serial', function(cb){
    spawn('particle', ['flash', '--serial', './target/fw.bin'],{
        stdio: 'inherit',
        cwd:'./fw'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('flash-online', function(cb){
    spawn('particle', ['flash', conf.device, '.'],{
        stdio: 'inherit',
        cwd:'./fw'
    }).on('exit', (code) => {
        cb(code)
    }); 
});


gulp.task('build',['compile-local','fw:host:test']);
gulp.task('travis', plugins.sequence('install-ext','bs:secrets', [ 'compile-online', 'fw:host:test']));
gulp.task('default', ['build']);