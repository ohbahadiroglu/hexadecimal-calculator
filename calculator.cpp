#include "calculator.h"
#include "ui_calculator.h"
#include <sstream>
#include <vector>


using namespace std;
int result = 0;
vector<string> tokens = {};
QString lastInput = "0";
QString dispLine = "";
bool equalsPressed = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);


    ui->Display->setText(QString::number(result));
    QPushButton *numButtons[16];
    for(int i = 0; i < 16; i++){
        QString butName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i],SIGNAL(released()),this,SLOT(InputButtonPressed()));
    }

    QPushButton *add;
    QPushButton *subtract;
    QPushButton *clear;
    QPushButton *equals;
    add = Calculator::findChild<QPushButton *>("Add");
    subtract = Calculator::findChild<QPushButton *>("Subtract");
    clear = Calculator::findChild<QPushButton *>("Clear");
    equals = Calculator::findChild<QPushButton *>("Equals");
    connect(add,SIGNAL(released()),this,SLOT(InputButtonPressed()));
    connect(subtract,SIGNAL(released()),this,SLOT(InputButtonPressed()));
    connect(clear,SIGNAL(released()),this,SLOT(ClearButtonPressed()));
    connect(equals,SIGNAL(released()),this,SLOT(EqualsButtonPressed()));
}
Calculator::~Calculator()
{
    delete ui;
}

void Calculator::InputButtonPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->Display->text();


    if (displayVal != "Syntax Error"){

        if (equalsPressed == false){
            dispLine += butVal;
            if ((lastInput == " + " || lastInput == " - ") && (butVal == " + " || butVal == " - ")){
                dispLine = "Syntax Error";
                ui->Display->setText(dispLine);
            }else{
                lastInput = butVal;
                ui->Display->setText(dispLine);
            }
        }else{
            if (butVal == " + " || butVal == " - "){
                equalsPressed = false;
                dispLine += butVal;
                if ((lastInput == " + " || lastInput == " - ") && (butVal == " + " || butVal == " - ")){
                    dispLine = "Syntax Error";
                    ui->Display->setText(dispLine);
                }else{
                    lastInput = butVal;
                    ui->Display->setText(dispLine);
                }
            }else{
                dispLine = butVal;
                lastInput = "0";
                result = 0;
                tokens.clear();
                equalsPressed = false;
                ui->Display->setText(dispLine);
            }
        }
    }
}
void Calculator::ClearButtonPressed(){
    dispLine = "";
    lastInput = "0";
    result = 0;
    tokens.clear();
    equalsPressed = false;
    ui->Display->setText(QString::number(result));
}
void Calculator::EqualsButtonPressed(){
    QString displayVal = ui->Display->text();
    if (displayVal != "Syntax Error"){
        string tmpLine = dispLine.toStdString();
        stringstream parsedDispLine(tmpLine);
        string input;
        parsedDispLine >> input;
        if (input == "+" || input == "-"){
            tokens.push_back("0");
        }
        tokens.push_back(input);
        while (parsedDispLine >> input) {
            tokens.push_back(input);
        }

        if (tokens.back() == "+" || tokens.back() == "-"){
            dispLine = "Syntax Error";
            ui->Display->setText(dispLine);
        }else{
            string hexStr = tokens.front();
            stringstream ss;
            ss << std::hex << hexStr;
            ss >> result;
            for (int i = 1; i < int(tokens.size()-1); i+=2 ){
                string hexStr = tokens.at(i+1);
                int x;
                stringstream ss;
                ss << std::hex << hexStr;
                ss >> x;
                if (tokens.at(i) == "+"){
                    result += x;
                }else{
                    result -= x;
                }
            }

            stringstream ssresult;
            if ( result >= 0 ){
                ssresult << hex << uppercase << result;
                string res = ssresult.str();
                dispLine = QString::fromStdString(res);
            }else{
                dispLine = "- ";
                result = (-1*result);
                ssresult << hex << uppercase << result;
                string res = ssresult.str();
                dispLine += QString::fromStdString(res);
            }
            ui->Display->setText(dispLine);
        }
        equalsPressed = true;
        tokens.clear();
    }
}
