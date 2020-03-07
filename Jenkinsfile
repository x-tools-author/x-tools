pipeline {
    agent any

    stages {
        stage('Setup environment') {
            steps {
                set PATH=C:\Qt\5.13.2\msvc2017\bin;%PATH%
                call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
            }
        }
        stage('Build') {
            steps {
                qmake -makefile -o Makefile QtSwissArmyKnife.pro
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying....'
            }
        }
    }
}

