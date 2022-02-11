pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'cd examples/1 && make'
            }
        }
        stage('Test') {
            steps {
                sh 'cd examples/1'
                sh 'ls'
                sh 'echo "Running..."'
                sh './a.out'
            }
        }
    }
}