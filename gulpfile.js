var path = require('path');
var spawn = require('child_process').spawn;
var fs = require('fs-extra')
var del = require('del');
var gulp = require('gulp');
var plugins = require('gulp-load-plugins')();

var pwd = process.cwd();

var conf = {
    fw: {
        out: 'fw/target'
    }
};

gulp.task('clean-make', function(cb) {
    spawn('make', ['clean', 'PLATFORM=photon', 'APPDIR=' + path.join(pwd, 'fw')],{
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
    spawn('make', ['all', 'PLATFORM=photon', 'APPDIR=' + path.join(pwd, 'fw')],{
        stdio: 'inherit',
        cwd:'../firmware/modules'
    }).on('exit', (code) => {
        cb(code)
    }); 

});

gulp.task('compile-local', function(cb){
    spawn('make' , [ /*'-s',*/ 'all', 'PLATFORM=photon', 'APPDIR=' + path.join(pwd, 'fw')],{
        stdio: 'inherit',
        cwd:'../firmware/main'
    }).on('exit', (code) => {
        cb(code)
    }); 

});

gulp.task('compile-online', function(cb){
    fs.mkdirs(conf.fw.out,function(err){
        spawn('particle', ['compile', 'photon', '.' ,  '--saveTo' ,  path.join('target' , 'fw.bin')],{
            stdio: 'inherit',
            cwd:'./fw'
        }).on('exit', (code) => {
            cb(code)
        }); 
    })
});

gulp.task('flash-local', function(cb){
    spawn('particle', ['flash', 'bt-photon-1', './target/fw.bin'],{
        stdio: 'inherit',
        cwd:'./fw'
    }).on('exit', (code) => {
        cb(code)
    }); 
});

gulp.task('flash-online', function(cb){
    spawn('particle', ['flash', 'bt-photon-1', '.'],{
        stdio: 'inherit',
        cwd:'./fw'
    }).on('exit', (code) => {
        cb(code)
    }); 
});


gulp.task('build',['compile-online'])
gulp.task('default', ['build']);