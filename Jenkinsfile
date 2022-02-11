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
                sh 'echo "Running..."'
                sh './a.out'
            }
        }
    }
}