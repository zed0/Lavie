#define QUIZ_TIME 20 //time between question and answer
#define QUIZ_NEXT 5 //time between answer and next question in the quiz

#include "quizPlugin.h"

vector<question> quizPlugin::questions;
int quizPlugin::quizTiming = QUIZ_TIME;
int quizPlugin::quizNextTiming = QUIZ_NEXT;
int quizPlugin::currentQuestion = 0;
bool quizPlugin::continuousQuestions = false;
