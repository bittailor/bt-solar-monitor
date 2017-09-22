var path = require('path');
var spawn = require('child_process').spawn;
var fs = require('fs-extra')
var del = require('del');
var gulp = require('gulp');
var plugins = require('gulp-load-plugins')();

var pwd = process.cwd();

var conf = {
    //device: "bt-electron-solar-monitor",
    //platform: "electron",
    device: "bt-photon-2",
    platform: "photon",
    fw: {
        out: 'fw/target'
    }
};

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


gulp.task('build',['compile-online'])
gulp.task('travis', ['compile-online']);
gulp.task('default', ['build']);