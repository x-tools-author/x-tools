pipeline {
    agent any

    stages {
        stage('Windows 32 bit release') {
            environment {
                SAK_BASEDIR = "${env.WORKSPACE}"
                SAK_QMAKE = "D:/qt/5.12/windows_5.12.6_msvc2017_32bits_static/bin/qmake"
                SAK_NMAKE = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x86/nmake"
            }
        
            steps {
                bat '${SAK_QMAKE} -makefile -o Makefile QtSwissArmyKnife.pro'
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

