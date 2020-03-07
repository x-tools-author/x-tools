Jenkinsfile (Declarative Pipeline)
pipeline {
    agent any
    stages {
        stage("build") {
            agent any
            steps {
                withSonarQubeEnv('sq.lan.com') {
                    bat 'echo hello Jenkinsfile'
                    bat 'set PATH=D:\qt\5.12\windows_5.12.6_msvc2017_32bits_static\bin;%PATH%'
                    bat 'call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"'
                    bat 'qmake -makefile -o Makefile QtSwissArmyKnife.pro'
                }
            }
        }
    }
}