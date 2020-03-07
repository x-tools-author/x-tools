pipeline {
    agent any

    stages {
        stage('Setup environment') {
            steps {
                echo 'Setup environment....'
                bat 'set PATH=C:/Qt/5.13.2/msvc2017/bin;%PATH%'
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

