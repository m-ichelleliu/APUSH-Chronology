#include <iostream>
#include <fstream> 
#include <cstdlib>
#include <vector>
using namespace std;

struct Question {
  string prompt;
  string answer; // accepted answer: either jenny's or year
  string year; // never changes, is just the year
  int wrong = 0;
  int right = 0;
  int placement; // exclusively for the ordering events one
  bool asked = false;
  Question (string prompto, string answero) {
    prompt = prompto;
    answer = answero;
    year = answero;
  }
  void Print() {
    cout << answer << " - " << prompt << endl;
  }
  void PrintRW() {
    Print();
    cout << right << " right, " << wrong << " wrong" << endl;
  }
  void ResetAll() { // not used anywhere yet
    wrong = 0; right = 0; asked = false;
  }
};

struct QuestionVect {
  vector<Question> questions;
  string title;
  bool in_use = false;
  void Print() {
    cout << "Chapter " << title << " - " << questions.size() << " terms" << endl;
  }
  void ResetAsked() {
    for (int i = 0; i < questions.size(); i++) {
      questions[i].asked = false;
    }
  }
};

void ClearScreen() {
  for (int i = 0; i < 50; i++) {
    cout << endl;
  }
}

// SET SELECTION

void SelectSets(vector<QuestionVect>* stuff, bool* jennysys) {
  cout << "Type the number of the chapter you would like to add or delete from your current study set. Type \"done\" when you are done, and \"terms\" to see all the terms. To toggle the use of Jenny's number system, type \"jenny\" (off by default)." << endl << endl;

  while (true) {
    // Printing.
    cout << "In Use:" << endl;
    for (int i = 0; i < (*stuff).size(); i++) {
      if ((*stuff)[i].in_use) {
        (*stuff)[i].Print();
      }
    }
    cout << endl;

    cout << "Not In Use:" << endl;
    for (int i = 0; i < (*stuff).size(); i++) {
      if (!(*stuff)[i].in_use) {
        (*stuff)[i].Print();
      }
    }
    cout << endl;

    // Input.
    cout << "Chapter to add/delete: ";

    string input;
    cin >> input;

    // If Finished.
    if (input == "done") {
      // make sure there aren't zero selected chapters
      int checksize = 0;
      for (int i = 0; i < (*stuff).size(); i++) {
        if ((*stuff)[i].in_use) {
          checksize += (*stuff)[i].questions.size();
        }
      }
      if (checksize != 0) {
        break;
      }
    }

    // View Terms.
    if (input == "terms") {
      cout << "All Terms In All Sets: " << endl << endl;

      for (int i = 0; i < (*stuff).size(); i++) {
        cout << "Chapter " << (*stuff)[i].title << " - " << (*stuff)[i].questions.size() << " terms" << endl;
        for (int j = 0; j < (*stuff)[i].questions.size(); j++) {
          (*stuff)[i].questions[j].Print();
        }
        cout << endl;
      }
      cout << endl << endl;
      continue;
    }

    // Toggling use of Jenny
    if (input ==  "jenny") {
      for (int i = 0; i < (*stuff).size(); i++) {
        for (int j = 0; j < (*stuff)[i].questions.size(); j++) {
          if (*jennysys) {
            (*stuff)[i].questions[j].answer = (*stuff)[i].questions[j].year;
          } else {
            (*stuff)[i].questions[j].answer = to_string(j+1);
          }
        }
      }
      *jennysys = !(*jennysys);
    }

    // Actually toggle use of chapter.
    for (int i = 0; i < (*stuff).size(); i++) {
      if ((*stuff)[i].title == input) {
        (*stuff)[i].in_use = !(*stuff)[i].in_use;
      }
    }
    ClearScreen();
  }

  ClearScreen();
}

int main() {
  srand(time(0));
  ifstream fin;
  fin.open("data");

  const int size = 5000; // idk

  vector<QuestionVect> stuff;

  string current_year;
  int stuffindex = -1;
  bool jennysys = 0;

  // GATHER THE DATA
  // turn the questions into a vector
  for (int i = 0; i < size; i++) {
    string input;
    getline(fin, input);
    // if the first character is a number make it the year or chapter
    if (input[0] - '0' >= 0 && input[0] - '0' <= 9) {
      // if its starting a new chapter, add to the vector
      if (input[input.size() - 1] == '.') {
        stuffindex ++;
        QuestionVect a;
        stuff.push_back(a);
        input.resize(input.size() - 1);
        stuff[stuffindex].title = input;
      } else {
        current_year = input;
      }
    // if not then put it in the year
    } else {
      if (input != "") {
        Question yo(input, current_year);
        stuff[stuffindex].questions.push_back(yo);
      }
    }
  }

  // CHOOSE SETS

  SelectSets(&stuff, &jennysys);
  int studysize = 0;
  for (int i = 0; i < stuff.size(); i++) {
    if (stuff[i].in_use) {
      studysize += stuff[i].questions.size();
    }
  }

  // CHOOSE A MODE OF STUDY

  while (true) {
    cout << "Type the number of the mode you would like to learn with. When you are done with that mode, type \"done\" to select a new one." << endl;
    cout << "1. View Set \n2. Multiple Choice \n3. Free Response \n4. Order Events (Like Quiz) \n5. View Stats \n6. Change Set" << endl << endl;

    cout << "Number of Mode: ";

    int choice;
    cin >> choice;
    cout << endl;

    ClearScreen();

    if (choice == 1) { // print all terms
      cout << studysize << " terms" << endl << endl;
      for (int i = 0; i < stuff.size(); i++) {
        if (stuff[i].in_use) {
          cout << "Chapter " << stuff[i].title << " - " << stuff[i].questions.size() << " terms" << endl;
          for (int j = 0; j < stuff[i].questions.size(); j++) {
            stuff[i].questions[j].Print();
          } 
          cout << endl;
        }
      }
    } else if (choice == 2) { // mult choice
      int questions_asked = 0, total_qs = 0, qwrongs = 0;
      const int choices = 3; // hardcoded number of answer choices but can change it here

      cout << "In multiple choice mode, type the letter of the correct answer, or the corresponding number 1-" << choices << ". Type \"done\" when you are done." << endl << endl;

      while (true) {
        // pick a question prompt 
        int setnum = rand () % stuff.size();
        
        // make sure its in the set
        if (!stuff[setnum].in_use) {
          continue;
        }

        int qnum = rand() % stuff[setnum].questions.size();

        // check if asked
        if (stuff[setnum].questions[qnum].asked) {
          continue;
        } 

        // pick answer choices
        string answer_choices[choices];
        int correct = rand() % choices;
        answer_choices[correct] = stuff[setnum].questions[qnum].answer;

        int acindex = 0;

        //randomly selecting le answa choices
        while (true) {
          // finish if we done with all the answers
          if (acindex == choices) {
            break;
          }

          // make sure we're not overwriting the correct one
          if (acindex == correct) {
            acindex++;
            continue;
          }

          // ok now pick a question so we can use its answer
          int asetnum = rand() % stuff.size();
          if (!stuff[asetnum].in_use) {
            continue;
          }
          int aqnum = rand() % stuff[asetnum].questions.size();

          // make sure it not repeat another answer
          bool rpet = false;
          for (int i = 0; i < choices; i++) {
            if (answer_choices[i] == stuff[asetnum].questions[aqnum].answer) {
              rpet = true;
              break;
            }
          }
          if (rpet) {
            continue;
          }

          // add to the answer array
          answer_choices[acindex] = stuff[asetnum].questions[aqnum].answer;
          acindex++;
        }

        // ask
        cout << total_qs+1 << ". " << stuff[setnum].questions[qnum].prompt << endl;
        for (int i = 0; i < choices; i++) {
          cout << char ('a' + i) << ". " << answer_choices[i] << endl;
        }
        cout << "Answer: ";

        string input;
        cin >> input;

        if (input == "done") {
          break;
        }

        // checc
        if (tolower(input[0]) - 'a' == correct || tolower(input[0]) - '1' == correct) {
          cout << "Correct!";
          stuff[setnum].questions[qnum].right++;
          stuff[setnum].questions[qnum].asked = true;
          questions_asked++;
        } else {
          cout << "Wrong, it was in " << stuff[setnum].questions[qnum].answer << ".";
          stuff[setnum].questions[qnum].wrong++;
          qwrongs++;
        }
        total_qs++;
        cout << endl << endl;

        // check if all asked
        if (questions_asked == studysize) {
          cout << "Congratulations, you have cleared all the terms once with a score of " << int(double(total_qs-qwrongs)*100/total_qs + .5) << "%!" << endl << endl;
          for (int i = 0; i < stuff.size(); i++) {
            if (stuff[i].in_use) {
              stuff[i].ResetAsked();
            }
          }
          questions_asked = 0;
          qwrongs = 0;
          total_qs = 0;
        }
      }
      ClearScreen();
    } else if (choice == 3) { // free answer
      cout << "In free response mode, type the correct answer. Type \"done\" when you are done." << endl << endl;
      int questions_asked = 0;
      int qwrongs = 0;
      int total_qs = 0;
      while (true) {
        int setnum = rand () % stuff.size();

        // make sure its in the set(s) being studied
        if (!stuff[setnum].in_use) {
          continue;
        }
        int qnum = rand() % stuff[setnum].questions.size();

        // check if asked already
        if (stuff[setnum].questions[qnum].asked) {
          continue;
        } 
        
        // ask the question now.
        cout << total_qs+1 << ". " << stuff[setnum].questions[qnum].prompt << ": ";
        string ans;
        cin >> ans;

        if (ans == "done") {
          break;
        }

        if (ans == stuff[setnum].questions[qnum].answer) {
          cout << "Correct!";
          stuff[setnum].questions[qnum].right++;
          stuff[setnum].questions[qnum].asked = true;
          questions_asked++;
        } else {
          cout << "Wrong, it was in " << stuff[setnum].questions[qnum].answer << ".";
          stuff[setnum].questions[qnum].wrong++;
          qwrongs++;
        }
        cout << endl << endl;

        total_qs++;

        // if all questionis have been asked then reset em
        if (questions_asked == studysize) {
          cout << "Congratulations, you have cleared all the terms once with a score of " << int(double(total_qs-qwrongs)*100/total_qs + .5) << "%!" << endl << endl;
          for (int i = 0; i < stuff.size(); i++) {
            if (stuff[i].in_use) {
              stuff[i].ResetAsked();
            }
          }
          questions_asked = 0;
          qwrongs = 0;
          total_qs = 0;
        }
      }
      ClearScreen();
    } else if (choice==4) { // order events
      // check if multichapter jenny
      if (jennysys) {
        int amt_used = 0; // um isnt this the same thing as study size? i should change it
        for (int i = 0; i < stuff.size(); i++) {
          if (stuff[i].in_use) {
            amt_used ++;
          }
        }
        if (amt_used > 1) {
          cout << "This mode does not work when studying multiple chapters with the Jenny system. If you understand the dangers, please type \"proceed\" to proceed. Otherwise, type \"done\" to exit." << endl;
          cout << "Answer: ";
          string aa;
          cin >> aa;
          ClearScreen();
          if (aa[0] == 'd') {
            continue;
          } 
        }
      }


      // actual
      cout << "In event order mode, look at the events and then type the NUMBER of each event in chronological order. Type \"done\" when you are done. (Note: You will not be asked to order events from the same year unless you are using the Jenny system. You will also not be asked about events that take a range of years.)" << endl << endl;

      while (true) {
        // set up array
        vector<Question> chron_years;
        const int event_num = 5;

        for (int i = 0; i < event_num; i++) {
          // pick the event
          int setnum;
          do {
            setnum = rand() % stuff.size();
          } while (!(stuff[setnum].in_use));
          
          bool all_num;
          int qnum;
          do {
            all_num = true;
            qnum = rand() % stuff[setnum].questions.size();
            
            
            //check to make sure its not one of 234-2349 those ones with dashes or smth
            for (int j = 0; j < stuff[setnum].questions[qnum].answer.length(); j++) {
              if (!((stuff[setnum].questions[qnum].answer[j] - '0' >= 0) && (stuff[setnum].questions[qnum].answer[j] - '0' <= 9))) {
                all_num=false;
              }
            }

            // check if asked 
            if (stuff[setnum].questions[qnum].asked) {
              all_num=false;
            }

            // check if year reused
            for (int j = 0; j < chron_years.size(); j++) {
              if (chron_years[j].answer == stuff[setnum].questions[qnum].answer) {
                all_num=false;
              }
            }

          } while (!all_num);

          stuff[setnum].questions[qnum].asked = true;
          stuff[setnum].questions[qnum].placement = i+1; // original number in prompt

          chron_years.push_back(stuff[setnum].questions[qnum]);
          cout << i+1 << ". " << chron_years[i].prompt << endl;
        }
        cout << endl;

        // input answers
        string oans[event_num];
        bool leave_mode = false;
        for (int i = 0; i < event_num; i++) {
          if (i/10 != 0) {
            cout << i/10;
          }
          switch (i%10) {
            case 0:
            cout << "1st";
            break;
            case 1:
            cout << "2nd";
            break;
            case 2:
            cout << "3rd";
            break;
            default:
            cout << i%10 << "th";
          }
          cout << " Event: ";
          cin >> oans[i];
          if (oans[i] == "done") {
            leave_mode = true;
            break;
          }
        }

        if (leave_mode) {
          break;
        }

        //user's answers
        int ans[event_num];
        for (int i = 0; i < event_num; i++) {
          ans[i] = stoi(oans[i]);
        }

        // bubble sort lol 
        bool no_swap=true;
        do {
          no_swap=true;
          for (int i = 0; i < event_num-1; i++) {
            if (stoi(chron_years[i].answer) > stoi(chron_years[i+1].answer)) {
              Question temp = chron_years[i];
              chron_years[i] = chron_years[i+1];
              chron_years[i+1] = temp;
              no_swap=false;
            }
          }
        } while (!no_swap);
        
        // check answers: place them in order, then compare the original number to the user's original number reponse
        bool wrong = false;
        for (int i = 0; i < event_num; i++) {
          if (!(chron_years[i].placement == ans[i])) {
            wrong = true;
          }
        }
        
        if (wrong) {
          cout << "Wrong, t";
        } else {
          cout << "Correct! T";
        }
        cout << "he order is as follows - " << endl;
          for (int i = 0; i < event_num; i++) {
            cout << chron_years[i].placement << ". (" << chron_years[i].answer << ") " << chron_years[i].prompt << endl;
          }

        cout << endl << endl;


      }
      ClearScreen();
    
    } else if (choice == 5) { // print all terms with r/w
      cout << studysize << " terms" << endl << endl;
      for (int i = 0; i < stuff.size(); i++) {
        if (stuff[i].in_use) {
          cout << "Chapter " << stuff[i].title << endl;
          for (int j = 0; j < stuff[i].questions.size(); j++) {
            stuff[i].questions[j].PrintRW();
          } 
          cout << endl;
        }
      }
    } else if (choice == 6) { // select sets
      SelectSets(&stuff, &jennysys);
      studysize = 0;
      for (int i = 0; i < stuff.size(); i++) {
        if (stuff[i].in_use) {
          studysize += stuff[i].questions.size();
        }
      }
      ClearScreen();
    } else { // how dare you not type a number 1-6
      cout << "Type the number of the mode." << endl;
      ClearScreen();
    }
  }
}