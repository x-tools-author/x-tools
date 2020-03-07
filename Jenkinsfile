pipeline {
    agent any

    stages {
        stage('Windows 32 bit release building') {
            steps {
                bat 'set PATH=D:/qt/5.12/windows_5.12.6_msvc2017_32bits_static/bin;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x86;%PATH%'
                bat 'qmake -makefile -o Makefile QtSwissArmyKnife.pro CONFIG+=release'
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

