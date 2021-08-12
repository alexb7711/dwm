pipeline {
	agent any
	stages {
		stage('build') {
			steps {
				echo 'building'
				sh 'make clean'
				sh 'make all'
			}
		}
	}
}
