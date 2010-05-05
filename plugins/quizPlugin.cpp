#define QUIZ_TIME 10 //time between question, answer and next question in the quiz

#include "quizPlugin.h"

vector<question> quizPlugin::questions;
int quizPlugin::quizTiming = QUIZ_TIME;
int quizPlugin::currentQuestion = 0;
bool quizPlugin::continuousQuestions = false;
