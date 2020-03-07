pipeline {
    agent any

    stages {
        stage('Setup environment') {
            steps {
                echo 'Setup environment....'
                bat 'set PATH=C:/Qt/5.13.2/msvc2017/bin;%PATH%'
                bat 'set PATH=C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build;%PATH%'
                bat 'vcvars32.bat'
            }
        }
        stage('Build') {
            steps {
                echo 'Deploying....'
                bat 'qmake -makefile -o Makefile QtSwissArmyKnife.pro'
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

