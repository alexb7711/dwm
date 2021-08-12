pipeline {
	agent any
	stages {
		stage ('checkout') {
			checkout master
		}

		stage('build') {
			echo 'building'
			sh 'make clean'
			sh 'make'
		}
	}
}
