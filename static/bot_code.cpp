#include "MyBot.h"
#include <dpp/dpp.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstdio>
#include <locale.h>

const std::string BOT_TOKEN = "";
int64_t guess_ans = 0;
using namespace std;
dpp::snowflake file_id;
dpp::snowflake account_id;
bool d = 0, a = 0;
int64_t in=0;
int64_t out=0;
int64_t mon=0;
int type[5] = { 0 };
struct Todolist
{
    int deadline;
    std::string task;
};
bool todo_compare(Todolist a, Todolist b);

int main() {

    dpp::cluster bot(BOT_TOKEN);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {

            //greeting
            dpp::slashcommand greeting("greeting", "Say Hello to the given username", bot.me.id);
            greeting.add_option(dpp::command_option(dpp::co_string, "username", "Please enter the username", true));
            bot.global_command_create(greeting);
            
            //reset
            dpp::slashcommand reset("reset", "Randomly generate an integer between 1 and 100", bot.me.id);
            bot.global_command_create(reset);
            
            //add
            dpp::slashcommand add("add", "add two given integers", bot.me.id);
            add.add_option(dpp::command_option(dpp::co_integer, "number_1", "Please enter an integer", true));
            add.add_option(dpp::command_option(dpp::co_integer, "number_2", "Please enter an integer", true));
            bot.global_command_create(add);
            
            //sub
            dpp::slashcommand sub("sub", "subtract two given integers", bot.me.id);
            sub.add_option(dpp::command_option(dpp::co_integer, "number_1", "Please enter an integer", true));
            sub.add_option(dpp::command_option(dpp::co_integer, "number_2", "Please enter an integer", true));
            bot.global_command_create(sub);
            
            //mul
            dpp::slashcommand mul("mul", "multiply two given integers", bot.me.id);
            mul.add_option(dpp::command_option(dpp::co_integer, "number_1", "Please enter an integer", true));
            mul.add_option(dpp::command_option(dpp::co_integer, "number_2", "Please enter an integer", true));
            bot.global_command_create(mul);
            
            //guess
            dpp::slashcommand guess("guess", "guess an integer between 1 and 100", bot.me.id);
            guess.add_option(dpp::command_option(dpp::co_integer, "number_guess", "Please enter an integer between 1 and 100", true));
            bot.global_command_create(guess);
            
            //write
            bot.global_command_create(dpp::slashcommand("write", "Make your own diary", bot.me.id));
            
            //read
            dpp::slashcommand read("read", "Read a of a specific date",  bot.me.id);
            read.add_option(dpp::command_option(dpp::co_string, "date", "please enter a date yyyymmdd", true));
            bot.global_command_create(read);
            dpp::snowflake file_id = read.application_id; //change to message
            
            //remove
            dpp::slashcommand remove("remove", "remove a diary of a specific date", bot.me.id);
            remove.add_option(dpp::command_option(dpp::co_string, "date", "please enter a date yyyymmdd", true));
            bot.global_command_create(remove);
            
            //account
            bot.global_command_create(dpp::slashcommand("account", "Organize your finances", bot.me.id));
            
            //check
            dpp::slashcommand check("check", "Check income and expense of a specific day", bot.me.id);
            check.add_option(dpp::command_option(dpp::co_string, "date", "please enter a date yyyymmdd", true));
            bot.global_command_create(check);
            dpp::snowflake account_id = check.application_id;
            
            //total_income
            dpp::slashcommand total_income("total_income", "Check total income so far", bot.me.id);
            bot.global_command_create(total_income);
            
            //total_expense
            dpp::slashcommand total_expense("total_expense", "Check total expense so far", bot.me.id);
            bot.global_command_create(total_expense);
            
            //save
            dpp::slashcommand save("save", "Check saved money", bot.me.id);
            bot.global_command_create(save);
            
            //analyze
            dpp::slashcommand analyze("analyze", "Analyze your expenses ", bot.me.id);
            bot.global_command_create(analyze);
            
            //todo
            dpp::slashcommand todo("todo", "Add a to-do task to the user's list", bot.me.id);
            todo.add_option(
                dpp::command_option(dpp::co_user, "user", "User whose list will be added", true));
            todo.add_option(
                dpp::command_option(dpp::co_string, "deadline", "Please enter a date(yyyymmdd)", true));
            todo.add_option(
                dpp::command_option(dpp::co_string, "todo", "Please enter the \"unique name\" of the to - do task", true));
            bot.global_command_create(todo);
            
            //list
            dpp::slashcommand list("list", "Show the user's to-do list", bot.me.id);
            list.add_option(
                dpp::command_option(dpp::co_user, "user", "User whose list will be showed", true));
            bot.global_command_create(list);

            //clear
            dpp::slashcommand clear("clear", "Clear the task which had been done", bot.me.id);
            clear.add_option(
                dpp::command_option(dpp::co_user, "user", "User whose list will be modify", true));
            clear.add_option(
                dpp::command_option(dpp::co_string, "todo", "Please enter the name of the completed task", true));
            bot.global_command_create(clear);
        }

     });

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::interaction interaction = event.command;
        dpp::command_interaction cmd_data = interaction.get_command_interaction();

        //---task1----------------------------------//
        //Greeting
        if (event.command.get_command_name() == "greeting") {
            std::string username = std::get<std::string>(event.get_parameter("username"));
            std::string mes = "Hello " + username + ", good morning!";
            event.reply(std::string(mes));
        }

        //---task2----------------------------------//
        //Add
        if (event.command.get_command_name() == "add") {
            int64_t num1 = get<std::int64_t>(event.get_parameter("number_1"));
            int64_t num2 = get<std::int64_t>(event.get_parameter("number_2"));
            int64_t ans = num1 + num2;
            event.reply("[Add]The result is " + to_string(num1) + " + " + to_string(num2) + " = " + to_string(ans));
        }
        //Sub
        if (event.command.get_command_name() == "sub") {
            int64_t num1 = get<std::int64_t>(event.get_parameter("number_1"));
            int64_t num2 = get<std::int64_t>(event.get_parameter("number_2"));
            int64_t ans = num1 - num2;
            event.reply("[Sub]The result is " + to_string(num1) + " - " + to_string(num2) + " = " + to_string(ans));
        }
        //Mul
        if (event.command.get_command_name() == "mul") {
            int64_t num1 = get<std::int64_t>(event.get_parameter("number_1"));
            int64_t num2 = get<std::int64_t>(event.get_parameter("number_2"));
            int64_t ans = num1 * num2;
            event.reply("[Mul]The result is " + to_string(num1) + " * " + to_string(num2) + " = " + to_string(ans));
        }

        //---task3----------------------------------//
        //Reset
        if (event.command.get_command_name() == "reset") {
            int a = 1;
            int b = 100;
            int x = rand() % (b - a + 1) + a;
            guess_ans = x;
            event.reply("[Guess Number] Reset Successful!");
            event.reply(to_string(guess_ans));
        }
        //Guess
        if (event.command.get_command_name() == "guess") {
            int64_t num = get<std::int64_t>(event.get_parameter("number_guess"));
            if (num == guess_ans) {
                event.reply("Bingo!");
            }
            else if (num > guess_ans) {
                event.reply("Guess a smaller number!");
            }
            else {
                event.reply("Guess a larger number!");
            }
        }

        //---task4----------------------------------//
        //Read
        if (event.command.get_command_name() == "read") {
            std::string date = std::get<std::string>(event.get_parameter("date"));
            std::ifstream file("diary\\"+date + ".txt");
            if (file.is_open()) {
                std::string title, content;
                std::getline(file, title); 
                std::getline(file, content, '\0');

                dpp::embed diary_embed = dpp::embed().
                    set_title(title).
                    set_color(dpp::colors::sti_blue).
                    add_field(
                        "Date:",
                        date
                    ).
                    add_field(
                        "Content:",
                        content,
                        true
                    ).
                    set_footer(dpp::embed_footer().set_text("My Diary at " + date)).
                    set_timestamp(time(0));
                dpp::message mes(file_id, diary_embed);
                event.reply(mes);
            }
            else {
                event.reply("Diary not found!!!!");
            }
        }
        //Remove
        if (event.command.get_command_name() == "remove") {
            std::string date = std::get<std::string>(event.get_parameter("date"));
            string file = "diary\\" + date + ".txt";
            int yn=remove(file.c_str());
            if (yn == 0) event.reply("Diary deleted successfully :)");
            else event.reply("Diary deletion failed :(");
            
        }
        //Write
        if (event.command.get_command_name() == "write") {
            d = 1;//write or account in on_form_submit
            dpp::interaction_modal_response modal("my_modal", "Please enter your diary");
            modal.add_component(
                dpp::component().
                set_label("Date(In forms of YYYYMMDD)").
                set_id("day").
                set_type(dpp::cot_text).
                set_placeholder("YYYYMMDD").
                set_min_length(8).
                set_max_length(8).
                set_text_style(dpp::text_short)
            );
            modal.add_row();
            modal.add_component(
                dpp::component().
                set_label("Title").
                set_id("topic").
                set_type(dpp::cot_text).
                set_placeholder("title").
                set_min_length(1).
                set_max_length(50).
                set_text_style(dpp::text_short)
            );
            modal.add_row();
            modal.add_component(
                dpp::component().
                set_label("Your diary").
                set_id("content").
                set_type(dpp::cot_text).
                set_placeholder("content").
                set_min_length(1).
                set_max_length(2000).
                set_text_style(dpp::text_paragraph)
            );
            event.dialog(modal);

        }

        //---custom-feature-account----------------------------------//
        if (event.command.get_command_name() == "account") {
            a = 1; //account or write in on_form_submit
            dpp::interaction_modal_response acc("my_modal", "Record your income and expense");
            acc.add_component(
                dpp::component().
                set_label("Date(In forms of YYYYMMDD)").
                set_id("day").
                set_type(dpp::cot_text).
                set_placeholder("YYYYMMDD").
                set_min_length(8).
                set_max_length(8).
                set_text_style(dpp::text_short)
            );
            acc.add_row();
            acc.add_component(
                dpp::component().
                set_label("Income").
                set_id("Income").
                set_type(dpp::cot_text).
                set_placeholder("200").
                set_min_length(1).
                set_max_length(50).
                set_text_style(dpp::text_short)
            );
            acc.add_row();
            acc.add_component(
                dpp::component().
                set_label("expense").
                set_id("expense").
                set_type(dpp::cot_text).
                set_placeholder("200").
                set_min_length(1).
                set_max_length(50).
                set_text_style(dpp::text_short)
            );
            acc.add_row();
            acc.add_component(
                dpp::component().
                set_label("expense_type").
                set_id("expense_type").
                set_type(dpp::cot_text).
                set_placeholder("food/clothing/housing/transportation/entertainment").
                set_min_length(1).
                set_max_length(50).
                set_text_style(dpp::text_short)
            );
            event.dialog(acc);

        }
        if (event.command.get_command_name() == "check") {
            std::string date = std::get<std::string>(event.get_parameter("date"));
            std::ifstream file("account\\" + date + ".txt");
            if (file.is_open()) {
                std::string income, expense;
                std::getline(file, income);
                std::getline(file, expense);
                dpp::embed account_embed = dpp::embed().
                    set_title(date).
                    add_field(
                        "Income:",
                        income
                    ).
                    add_field(
                        "Expense:",
                        expense
                    ).
                    set_footer(dpp::embed_footer().set_text("My Account at " + date)).
                    set_timestamp(time(0));
                dpp::message mes(file_id, account_embed);
                event.reply(mes);
            }
            else {
                event.reply("Account not found!!!!");
            }
        }
        if (event.command.get_command_name() == "total_income") {
            event.reply(to_string(in));
        }
        if (event.command.get_command_name() == "total_expense") {
            event.reply(to_string(out));
        }
        if (event.command.get_command_name() == "save") {
            event.reply(to_string(mon));
        }
        if (event.command.get_command_name() == "analyze") {
            dpp::message total;
            int food=round(((float)type[0] / out)*10000)/100, clothing=round(((float)type[1] / out) * 10000)/100, housing=round(((float)type[2] / out) * 10000)/100, transportation=round(((float)type[3] / out) * 10000)/100, entertainment=round(((float)type[4] / out) * 10000)/100;
            total.set_content("Food:" + to_string(food) + '%' + '\n' + "Clothing:" + to_string(clothing) + '%' + '\n' + "Housing:" + to_string(housing) + '%' + '\n' + "Transportation:" + to_string(transportation) + '%' + '\n' + "Entertainment:" + to_string(entertainment) + '%');
            event.reply(total);        
        }

        //---custom-feature-todo-list----------------------------------//
        if (event.command.get_command_name() == "todo") {
            dpp::user user = interaction.get_resolved_user(
                cmd_data.get_value<dpp::snowflake>(0)
            );
            std::string deadline = std::get<std::string>(event.get_parameter("deadline"));
            std::string todotask = std::get<std::string>(event.get_parameter("todo"));
            std::string the_list = "todo\\" + user.username + ".txt";
            std::fstream todofile(the_list);
            bool todo = true;
            todotask = " " + todotask;

            if (deadline.size() != 8) {
                todo = false;
                event.reply("**!!!** Invaid Date X_X");
            }else if(todofile.is_open()) {                
                std::string dl;
                std::string list_task;
                while (todofile >> dl) {
                    std::getline(todofile, list_task);
                    cout << todotask << '/' << list_task << ' ';
                    if (todotask == list_task) {
                        todo = false;
                        event.reply("**!!!** Existed Todo Task X_X");
                    }
                }todofile.close();
            }
            if(todo) {
                todofile.open(the_list, std::ios_base::app);
                todofile << deadline << todotask << '\n';
                todofile.close();
                event.reply(user.get_mention() +
                    "\nThere is a new task in your list: " + todotask + "\nDeadline: " + deadline);
            }                       
        }
        if (event.command.get_command_name() == "list") {
            dpp::user user = interaction.get_resolved_user(
                cmd_data.get_value<dpp::snowflake>(0)
            );
            std::string the_list = "todo\\" + user.username + ".txt";             
            std::fstream todofile(the_list);

            if (todofile.is_open()) {
                int idx = 0; //count task                
                std::string dl_stoi;
                std::string the_list_content = user.username;
                std::vector<Todolist> tosort;
                Todolist t;

                while (todofile >> dl_stoi) {
                    t.deadline = std::stoi(dl_stoi);
                    std::getline(todofile, t.task);
                    tosort.push_back(t);
                    idx++;
                }todofile.close();

                if (idx) {
                    std::sort(tosort.begin(), tosort.end(), todo_compare);
                    todofile.open(the_list, std::ios_base::out);
                    for (int i = 0; i < idx; i++) {
                        the_list_content = the_list_content + "\n> " + std::to_string(tosort[i].deadline) + ' ' + tosort[i].task;
                        todofile << tosort[i].deadline << tosort[i].task << '\n';
                    }
                    event.reply(the_list_content);
                    todofile.close();
                }
                else {
                    the_list_content = the_list_content + "\n> " + std::to_string(tosort[0].deadline) + ' ' + tosort[0].task;
                    event.reply(the_list_content);
                }
            }else event.reply(std::string("**!!!** Do not find their list '^'"));

        }        
        if (event.command.get_command_name() == "clear") {
            dpp::user user = interaction.get_resolved_user(
                cmd_data.get_value<dpp::snowflake>(0)
            );
            std::string todotask = std::get<std::string>(event.get_parameter("todo"));
            std::string the_list = "todo\\" + user.username + ".txt";
            std::fstream todofile(the_list);
            todotask = " " + todotask;
            bool clear = false;
            if (todofile.is_open()) {
                int idx = 0; //count task                
                std::string dl_stoi;
                std::vector<Todolist> all_list;
                Todolist t;

                while (todofile >> dl_stoi) {
                    t.deadline = std::stoi(dl_stoi);
                    std::getline(todofile, t.task);
                    cout << todotask << '/' << t.task << ' ';
                    if (todotask == t.task) {
                        clear = true;
                        continue;
                    }
                    all_list.push_back(t);
                    idx++;
                }todofile.close();
                if (clear) {
                    if (idx) {  //avoid void list
                        todofile.open(the_list, std::ios_base::out);
                        for (int i = 0; i < idx; i++) {
                            todofile << all_list[i].deadline << all_list[i].task << '\n';
                        }
                        todofile.close();                        
                    }
                    else {
                        const char* Path = nullptr;
                        Path = the_list.c_str();
                        remove(Path);
                    }
                    event.reply("Clear successfully :D");
                }else event.reply(std::string("**!!!** Do not find the task '^' "));

            }else event.reply(std::string("**!!!** Do not find their list '^'"));
        }  

     });

    bot.on_form_submit([&](const dpp::form_submit_t& event) {
            if (d) { //write
                std::string date = std::get<std::string>(event.components[0].components[0].value);
                std::string title = std::get<std::string>(event.components[1].components[0].value);
                std::string content = std::get<std::string>(event.components[2].components[0].value);
                
                ofstream file("diary\\" + date + ".txt"); //oprn the file in diaery
                if (file.is_open()) {
                    file << title << "\n";
                    file << content << "\n";
                    file.close();
                }
                dpp::message m;
                m.set_content("Date:"+date+'\n'+"Title:"+title+'\n' + "Content:" + '\n' + content);
                event.reply(m);
                d = 0;
            }
            if (a) { //account
                dpp::message m;
                string date = get<std::string>(event.components[0].components[0].value);
                string income = get<std::string>(event.components[1].components[0].value);
                string expense = get<std::string>(event.components[2].components[0].value);
                string expense_type = get<std::string>(event.components[3].components[0].value);
                in += atoi(income.c_str());
                out += atoi(expense.c_str());
                mon = (in - out);
                if (expense_type == "food") {
                    type[0] += atoi(expense.c_str());
                }else if (expense_type == "clothing") {
                    type[1] += atoi(expense.c_str());
                }
                else if (expense_type == "housing") {
                    type[2] += atoi(expense.c_str());
                }
                else if (expense_type == "transportation") {
                    type[3] += atoi(expense.c_str());
                }
                else if (expense_type == "entertainment") {
                    type[4] += atoi(expense.c_str());
                }
                
                ifstream file("account\\" + date + ".txt"); //open the file in account
                if (file.is_open()) {
                    int64_t newincome=atoi(income.c_str()), newexpense=atoi(expense.c_str());
                    std::string la_income, la_expense;
                    std::getline(file, la_income);
                    std::getline(file, la_expense);
                    newincome += atoi(la_income.c_str());
                    newexpense += atoi(la_expense.c_str());
                    string f = "account" + date + ".txt";
                    remove(f.c_str());
                    ofstream newfile("account\\" + date + ".txt");
                    newfile << newincome << "\n";
                    newfile << newexpense << "\n";
                    m.set_content("Date:" + date + '\n' + "Income:" + to_string(newincome) + '\n' + "Expense:" + to_string(newexpense));
                }
                else {
                    ofstream file("account\\" + date + ".txt");
                    file << income << "\n";
                    file << expense << "\n";
                    file.close();
                    m.set_content("Date:" + date + '\n' + "Income:" + income + '\n' + "Expense:" + expense);
                }
                event.reply(m);
                a = 0;
            }           
     });

    bot.start(dpp::st_wait);
    return 0;
}

bool todo_compare(Todolist a, Todolist b) {
    if (a.deadline == b.deadline) return a.task < b.task;
    else return a.deadline < b.deadline;
}