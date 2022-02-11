pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'cd examples/1 && make'
                archiveArtifacts artifacts: 'bin/Debug/*', fingerprint: true
            }
        }
        stage('Test') {
            steps {
                sh 'echo "Running..."'
                sh './a.out'
            }
        }
    }
}