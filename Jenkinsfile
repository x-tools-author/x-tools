pipeline {
    agent any

    stages {
        stage('Windows 32 bit release') {       
            steps {
                bat 'qmake -makefile -o Makefile QtSwissArmyKnife.pro&&nmake'
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

