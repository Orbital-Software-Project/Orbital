#pragma once

#include <atomic>

struct Percentage {

	Percentage() : Percentage(0, 0) {}

	Percentage(int maxSteps, int currStep) : maxSteps(maxSteps), currStep(currStep) {
		this->calcPercentage();
	}

	void SetMaxSteps(int maxSteps) {
		this->maxSteps = maxSteps;

		this->calcPercentage();
	}

	int GetMaxSteps() {
		return this->maxSteps;
	}

	void SetCurrstep(int currStep) {
		this->currStep = currStep;

		this->calcPercentage();
	}

	int GetCurrStep() {
		return this->currStep;
	}


	int P;


private:
	void calcPercentage() {
		if (this->maxSteps > 0) {
			this->P = static_cast<int>((100.0f / this->maxSteps) * this->currStep);

		}
		else {
			this->P = 100;
		}
	}


private:
	

	int maxSteps = 0;

	int currStep = 0;
};