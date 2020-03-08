pipeline {
    agent any

    stages {
        stage('Windows msvc 32 bit release') {
            environment {
                SAK_WORKSPACE = "${env.WORKSPACE}"
                QT_BIN = "D:/qt/5.12/windows_5.12.7_msvc2017_32bits_static/bin"
                VCVARS_ENV = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build"
                BUILD_DIR = "build"
            }

            steps {
                powershell 'echo $env:BUILD_DIR'
                powershell 'If(TEST-Path $env:BUILD_DIR){Remove-Item $env:BUILD_DIR -recurse;mkdir $env:BUILD_DIR}Else{mkdir $env:BUILD_DIR}'
                bat "cd %BUILD_DIR%&&set PATH=%QT_BIN%;%VCVARS_ENV%;%PATH%&&vcvars32&&qmake ../QtSwissArmyKnife.pro&&nmake"
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

