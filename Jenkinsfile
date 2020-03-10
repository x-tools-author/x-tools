pipeline {
    agent any

    stages {
        stage('Windows msvc 32 bit release') {
            environment {
                SAK_WORKSPACE = "${env.WORKSPACE}"
                QT_BIN = "${env.QT_DEFAULT_BIN_WIN32}"
                VC_PATH = "${env.VC17_32_PATH}"
                VCVARS = "${env.VCVARS32}"
                BUILD_DIR = "build_win32"
                MAKE = "${env.MAKE_WIN}"
            }
        
            steps {
                powershell 'echo $env:BUILD_DIR'
                powershell 'If(Test-Path $env:BUILD_DIR){Remove-Item $env:BUILD_DIR -recurse;mkdir $env:BUILD_DIR}Else{mkdir $env:BUILD_DIR}'
                bat "cd %BUILD_DIR%&&set PATH=%QT_BIN%;%VC_PATH%;%PATH%&&%VCVARS%&&qmake ../QtCRCCalculator.pro&&%MAKE%"
            }
        }
        
        stage('Windows msvc 64 bit release') {
            environment {
                SAK_WORKSPACE = "${env.WORKSPACE}"
                QT_BIN = "${env.QT_DEFAULT_BIN_WIN64}"
                VC_PATH = "${env.VC17_64_PATH}"
                VCVARS = "${env.VCVARS64}"
                BUILD_DIR = "build_win64"
                MAKE = "${env.MAKE_WIN}"
            }
        
            steps {
                powershell 'echo $env:BUILD_DIR'
                powershell 'If(Test-Path $env:BUILD_DIR){Remove-Item $env:BUILD_DIR -recurse;mkdir $env:BUILD_DIR}Else{mkdir $env:BUILD_DIR}'
                bat "cd %BUILD_DIR%&&set PATH=%QT_BIN%;%VC_PATH%;%PATH%&&%VCVARS%&&qmake ../QtCRCCalculator.pro&&%MAKE%"
            }
        }

        stage('Collecting archive artifacts'){
            steps {
                archiveArtifacts '**/release/*.exe'
            }
        }
        
        stage('Build') {
            steps {
                echo 'Deploying....'
            }
        }
        stage('Test') {
            steps {
                echo 'Test....'
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying....'
            }
        }
    }
}

