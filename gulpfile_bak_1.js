var os = require('os');
var path = require('path');
import {globby} from 'globby';
var spawn = require('child_process').spawn;
var fs = require('fs-extra')
var del = require('del');
var gulp = require('gulp');
var plugins = require('gulp-load-plugins')();
var Q = require('q');
var Particle = require('particle-api-js');
var notifier  = require('node-notifier');
var particle = new Particle();

var secrets = require('./secrets');

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

function debugSolarWebHooks() {
    return particle
        .listWebhooks({auth: getParticleAccessToken()})
        .then((data) => {
            var hooks = data.body;         
            plugins.util.log(`hooks :`, hooks);
        });   
}

function deleteSolarWebHooks() {
    return particle
        .listWebhooks({auth: getParticleAccessToken()})
        .then((data) => {
            var hooks = data.body;
            var solarHooks = hooks.filter(hook => hook.event.startsWith('solar/'));
            plugins.util.log(`Found ${solarHooks.length} solar hooks`);
            var deletes = solarHooks.map((hook) => {
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
            var creates = files.map((file)=>{
                return sh('particle',['webhook', 'create', file], null, [process.stdin, 'ignore', process.stderr]);
            });
            return Promise.all(creates);
        })
        ; 
}

//----

gulp.task('web::hooks::debug', () => {
    return debugSolarWebHooks();
});

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

gulp.task('install-composer', function(){    
    return sh('composer',['install'],conf.web.api) 
});

gulp.task('install',['install-ext', 'install-composer']);


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
        // notifier.notify('Test run OK');
        return sh('make', ['check'], out);
    });
});

//----

// xdebug.remote_enable = 1
// xdebug.remote_autostart = 1

gulp.task('web:api:test', () => {
    var cwd = conf.web.api;
    return sh('php',[
        '-d', 'xdebug.remote_enable=1',
        '-d', 'xdebug.remote_autostart=1',
        'vendor/bin/phpunit',
        'tests'
    ],cwd);
    //return sh('vendor/bin/phpunit', ['tests'], cwd);
});

gulp.task('web:api:serve', () => {
    var cwd = path.join(conf.web.api,'public');
    return sh('php', ['-S', 'localhost:8080'], cwd);
});

gulp.task('web:api:deploy', () => {
    return sh('lftp', [ '-u', `${secrets.solar.hosting.user},${secrets.solar.hosting.pw}`, '-e', `set ftp:ssl-allow no; mirror --no-symlinks --reverse ${path.join(pwd,'web-api/solar-api')} /solar; exit`, 'ftp.bockmattli.ch' ]);
});

gulp.task('web:ui:deploy', () => {
    return sh('lftp', [ '-u', `${secrets.solar.hosting.user},${secrets.solar.hosting.pw}`, '-e', `set ftp:ssl-allow no; mirror --no-symlinks --reverse ${path.join(pwd,'web-ui')} /solardaten; exit`, 'ftp.bockmattli.ch' ]);
});

//----

gulp.task('clean-app', function(cb) {
    spawn('make', ['clean', 'PLATFORM=' + conf.platform, 'APPDIR=' + path.join(pwd, 'fw')],{
        stdio: 'inherit',
        cwd:'../firmware/main'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('clean-core', function(cb) {
    spawn('make', ['clean', 'PLATFORM=' + conf.platform],{
        stdio: 'inherit',
        cwd:'../firmware/modules'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('clean-dir', function () {
    return del([
        conf.fw.out
    ]);
});

gulp.task('clean',['clean-dir','clean-core','clean-app'])

gulp.task('compile-core', function(cb){
    spawn('make', ['all', 'PLATFORM=' + conf.platform],{
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
    fs.mkdirs(conf.fw.out,function(){
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

//gulp.task('build',  plugins.sequence('fw:host:test'));
//gulp.task('build',  plugins.sequence('compile-local'));
gulp.task('build',  plugins.sequence('fw:host:test','compile-local'));
gulp.task('travis', plugins.sequence('install', 'bs:secrets', 'fw:host:test', 'compile-online', 'web:api:test'));
gulp.task('default', ['build']);