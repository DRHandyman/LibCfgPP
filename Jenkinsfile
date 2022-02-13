pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh 'echo "Building..."'
        sh 'cd \'./examples/Reading strings and sections/\' && make'
      }
    }
  }
}