pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh 'echo "Building..."'
        sh 'cd \'./examples/Reading lines and sections/\' && make'
      }
    }

  }
}