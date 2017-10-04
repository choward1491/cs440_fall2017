pipeline {
    agent any 

    stages {
	state('Clean') {
	    steps {
		sh 'cd MP1; make clean'
	    }
	}

        stage('Build') { 
            steps { 
                sh 'cd MP1; make' 
            }
        }
        stage('Test'){
            steps {
                sh 'cd MP1; ./bin/mp_exec'
                junit 'reports/**/*.xml' 
            }
        }
        stage('Deploy') {
            steps {
                sh 'echo "Deploy me!"'
            }
        }
    }
}
