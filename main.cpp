#include <bits/stdc++.h>
using namespace std;
#define loop(i, s, n) for(int i=s;i<n;i++)
#define sz(s) (int)s.size()
vector<string> lines(string &line)
{
	istringstream in(line);
	vector<string> ret;
	string temp;
	loop(i,0,sz(line))
	{
		if(line[i]==',')
		{ret.push_back(temp);temp.clear();}
		else
			temp.push_back(line[i]);
	}
	return ret;
}
int to_int(string &temp)
{
	istringstream iss(temp);
	int n;
	iss>>n;
	return n;
}
vector<string> read_file(string file)
{
	vector<string> ret;
	string temp;
	ifstream in(file);
	if (in.fail()) {
		cout << "\n\nERROR: Can't open the file\n\n";
		return ret;
	}
	while(getline(in,temp))
	{ if(temp.empty())continue;ret.push_back(temp);}
	in.close();
	return ret;
}
void write_file(string file, vector<string>&lines,int append)
{
	auto status=ios::in|ios::out|ios::app;
	if(!append)
		status=ios::in|ios::out|ios::trunc;
	fstream out(file.c_str(),status);
	if (out.fail()) {
		cout << "\n\nERROR: Can't write the file\n\n";
		return;
	}
	for(auto &i:lines)
		out<<i<<"\n";
	out.close();
}
struct question
{
	int question_id;
	int parent_id;
	int from;
	int to;
	int anonymous;
	string question_text;
	string answer_text;
	question()
	{
		question_id=parent_id=from=to=-1;
		anonymous=1;
	}
	question (string &line)
	{
		vector<string> temp=lines(line);
		question_id = to_int(temp[0]);
		parent_id = to_int(temp[1]);
		from = to_int(temp[2]);
		to = to_int(temp[3]);
		anonymous = to_int(temp[4]);
		question_text = temp[5];
		answer_text = temp[6];
	}
	string to_string() {
		ostringstream oss;
		oss << question_id << "," << parent_id << "," << from << "," << to<< "," << anonymous << "," << question_text << "," << answer_text<<",";
		return oss.str();
	}
	void print_to_question() {
		string prefix;
		if (parent_id != -1)
			prefix = "\tThread: ";

		cout << prefix << "Question Id (" << question_id << ")";
		if (!anonymous)
			cout << " from user id(" << from << ")";
		cout << "\t Question: " << question_text << "\n";

		if (!answer_text.empty())
			cout << prefix << "\tAnswer: " << answer_text << "\n";
		cout << "\n";
	}
	void print_from_question() {
		cout << "Question Id (" << question_id << ")";
		if (!anonymous)
			cout << " !AQ";

		cout << " to user id(" << to << ")";
		cout << "\t Question: " << question_text;

		if (!answer_text.empty())
			cout << "\tAnswer: " << answer_text << "\n";
		else
			cout << "\tNOT Answered YET\n";
	}
	void print_feed_question() {
		if (parent_id != -1)
			cout << "Thread Parent Question ID (" << parent_id << ") ";

		cout << "Question Id (" << question_id << ")";
		if (!anonymous)
			cout << " from user id(" << from << ")";

		cout << " To user id(" << to << ")";

		cout << "\t Question: " << question_text << "\n";
		if (!answer_text.empty())
			cout << "\tAnswer: " << answer_text <<"\n";
	}
};
struct user
{
	int user_id;
	string user_name;
	string password;
	string name;
	string email;
	int anonymous;
	vector<int> questions_from_me;
	map<int,vector<int>> questions_to_me;
	user()
	{
		anonymous=user_id=-1;

	}
	user(string line)
	{
		vector<string> temp=lines(line);
		user_id= to_int(temp[0]);
		user_name=temp[1];
		password=temp[2];
		name=temp[3];
		email=temp[4];
		anonymous= to_int(temp[5]);
	}
	string to_string()
	{
		ostringstream out;
		out << user_id << "," << user_name << "," << password << "," << name << "," << email << "," << anonymous<<",";
		return out.str();
	}
	void print()
	{
		cout << "User " << user_id << ", " << user_name << " " << password << ", " << name << ", " << email << endl;
	}
};
struct question_op
{
	map<int,question> questions_id;
	map<int,vector<int>> questions_id_thread;
	int last_id;
	question_op(){
		last_id = 0;
	}
	void load_database() {
		last_id = 0;
		questions_id.clear();
		questions_id_thread.clear();
		vector<string> lines = read_file("questions.txt");
		for (auto &line : lines) {
			question cur(line);
			last_id = max(last_id, cur.question_id);

			questions_id[cur.question_id] = cur;

			if (cur.parent_id == -1)
				questions_id_thread[cur.question_id].push_back(cur.question_id);
			else
				questions_id_thread[cur.parent_id].push_back(cur.question_id);
		}
	}
	void fill_user_questions(user &cur_user)
	{
		cur_user.questions_from_me.clear();
		cur_user.questions_to_me.clear();
		for(auto i:questions_id_thread)
		{
			for(auto question_ids:i.second)
			{
				question &cur_question=questions_id[question_ids];
				if(cur_question.from==cur_user.user_id)
				{
					cur_user.questions_from_me.push_back(cur_question.question_id);
				}
				if(cur_question.to==cur_user.user_id)
				{
					if(cur_question.parent_id==-1)
						cur_user.questions_to_me[cur_question.question_id].push_back(cur_question.question_id);
					else
						cur_user.questions_to_me[cur_question.parent_id].push_back(cur_question.question_id);
				}
			}
		}
	}
	void print_questions_to_user(user cur_user)
	{
		cout << "\n";
		if (cur_user.questions_to_me.empty())
			cout << "No Questions"<<endl;
		for(auto pair:cur_user.questions_to_me)
		{
			for(auto question_id:pair.second)
			{
				question &cur_question =questions_id[question_id];
				cur_question.print_to_question();
			}
		}
	}
	void print_questions_from_user(user cur_user)
	{
		cout << "\n";
		if (cur_user.questions_from_me.empty())
			cout << "No Questions"<<endl;
		for(auto question_id:cur_user.questions_from_me)
		{
			question &cur_question=questions_id[question_id];
			cur_question.print_from_question();
		}
	}
	int check_question_id(user cur_user,int from_or_to)
	{
		cout<<"Enter Question id or -1 to cancel: ";
		int id;
		cin >> id;
		if(id==-1)
			return -1;
		cout<<endl;
		if(!questions_id.count(id))
		{
			cout<<"No question with such id..Try again."<<endl;
			return check_question_id(cur_user,from_or_to);
		}
		vector<int> temp={questions_id[id].to,questions_id[id].from};
		if (temp[from_or_to] != cur_user.user_id)
		{
			cout<< "You can't access others questions...Choose a question you asked."<< endl;
			return check_question_id(cur_user,from_or_to);
		}
		return id;
	}
	int check_parent_question()
	{
		cout<<"For thread question: Enter Question id or -1 for new question: ";
		int id;
		cin >> id;
		if(id==-1)
			return -1;
		cout<<endl;
		if(!questions_id_thread.count(id))
		{
			cout<<"No thread question with such id..Try again."<<endl;
			return check_parent_question();
		}
		return id;
	}
	void answer_question(user &cur_user)
	{
		int id=check_question_id(cur_user,0);
		if(id==-1)
			return;
		question &cur_question=questions_id[id];
		cur_question.print_to_question();
		if (!cur_question.answer_text.empty())
			cout << "\nWarning: Already answered. Answer will be updated\n";
		cout<<"Enter your answer: ";
		getline(cin,cur_question.answer_text);
		getline(cin,cur_question.answer_text);
		cout<<endl;
	}
	void delete_question(user &cur_user)
	{
		int id=check_question_id(cur_user,1);
		if(id==-1)
			return;
		question cur_question=questions_id[id];
		if(~cur_question.parent_id)
		{
			questions_id.erase(id);
			vector<int> &the_thread_containing_this_question=questions_id_thread[cur_question.parent_id];
			the_thread_containing_this_question.erase(find(the_thread_containing_this_question.begin(),the_thread_containing_this_question.end(),id));
		}
		else
		{
			for(auto id_to_delete:questions_id_thread[id])
			{
				questions_id.erase(id_to_delete);
			}
			questions_id_thread.erase(id);
		}
	}
	void ask_question(user &cur_user,pair<int,int> &to_user)
	{
		question new_question;
		if(to_user.second==1)
		{
			cout<< "Choose 1 if you want an anonymous question 0 otherwise: ";
			cin>>new_question.anonymous;
		}
		else
		{
			cout<< "Note Anonymous questions are not allowed for this user."<< endl;
			new_question.anonymous=0;
		}
		new_question.parent_id=check_parent_question();
		cout << "Enter question text: ";
		getline(cin, new_question.question_text);
		getline(cin, new_question.question_text);
		new_question.to=to_user.first;
		new_question.from=cur_user.user_id;
		new_question.question_id = ++last_id;
		questions_id[new_question.question_id]=new_question;
		if(new_question.parent_id==-1)
			questions_id_thread[new_question.question_id].push_back(new_question.question_id);
		else
			questions_id_thread[new_question.parent_id].push_back(new_question.question_id);
	}
	void list_feed()
	{
		for (auto &pair : questions_id) {
			question &cur_question = pair.second;

			if (cur_question.answer_text.empty())
				continue;
			cur_question.print_feed_question();
		}
	}
	void update_database() {
		vector<string> lines;

		for (auto &pair : questions_id)
			lines.push_back(pair.second.to_string());

		write_file("questions.txt", lines, false);
	}
};
struct user_op
{
	map<string,user> users;
	user current_user;
	int last_id;
	user_op()
	{
		last_id=0;
	}
	void load_database()
	{
		last_id=0;
		users.clear();
		vector<string> temp=read_file("users.txt");
		for(auto &i:temp)
		{
			user cur(i);
			users[cur.user_name]=cur;
			last_id=max(last_id,cur.user_id);
		}
	}
	void access_system() {
		cout<<"Menu:   "<<endl;
		cout<<"1: Login"<<endl<<"2: Sign Up"<<endl;
		cout<<"Choose an option: ";
		int n;
		cin>>n;
		if (n == 1)
			login();
		else
			sign_up();
	}
	void login() {
		load_database();
		while (true) {
			cout << "Enter user name & password: ";
			cin >> current_user.user_name >> current_user.password;
			if (!users.count(current_user.user_name)) {
				cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			user user_exist = users[current_user.user_name];

			if (current_user.password != user_exist.password) {
				cout << "\nInvalid user name or password. Try again\n\n";
				continue;
			}
			current_user = user_exist;
			break;
		}
	}
	void sign_up() {
		load_database();
		while (true) {
			cout << "Enter user name. (No spaces): ";
			cin >> current_user.user_name;

			if (users.count(current_user.user_name))
				cout << "Already used. Try again\n";
			else
				break;
		}
		cout << "Enter password: ";
		cin >> current_user.password;

		cout << "Enter name: ";
		cin >> current_user.name;

		cout << "Enter email: ";
		cin >> current_user.email;

		cout << "Allow anonymous questions? (0 or 1): ";
		cin >> current_user.anonymous;
		current_user.user_id = ++last_id;
		users[current_user.user_name] = current_user;
		update_database(current_user);
	}
	void list_users_names_ids() {
		for (auto &pair : users)
			cout << "ID: " << pair.second.user_id << "\t\tName: " << pair.second.name << "\n";
	}
	pair<int, int> read_user_id() {
		int user_id;
		cout << "Enter User id or -1 to cancel: ";
		cin >> user_id;

		if (user_id == -1)
			return make_pair(-1, -1);

		for (auto &pair : users) {
			if (pair.second.user_id == user_id)
				return make_pair(user_id, pair.second.anonymous);
		}

		cout << "Invalid User ID. Try again\n";
		return read_user_id();
	}
	void update_database(user &user) {
		string line = user.to_string();
		vector<string> lines;
		lines.push_back(line);
		write_file("users.txt", lines,true);
	}
};
struct ask_system
{
	user_op user_ops;
	question_op question_ops;
	void load_database(bool fill_user_questions=false)
	{
		user_ops.load_database();
		question_ops.load_database();
		if(fill_user_questions)
			question_ops.fill_user_questions(user_ops.current_user);
	}
	void run()
	{
		load_database();
		user_ops.access_system();
		question_ops.fill_user_questions(user_ops.current_user);
		while(true)
		{
			int n = 0;
			while (!n)
			{
				cout << "Menu: " << endl;
				cout << "1: Print Questions To Me" << endl;
				cout << "2: Print Questions From Me"<< endl;
				cout << "3: Answer Question" << endl;
				cout << "4: Delete Question" << endl;
				cout << "5: Ask Question" << endl;
				cout << "6: List System Users" << endl;
				cout << "7: Feed" << endl;
				cout << "8: Logout" << endl;
				cin >> n;
				if (n >= 1 && n <= 8)
					break;
				n = 0;
				cout << "Invalid number...Try again"<< endl;
			}
			load_database(true);
			if (n == 1)
				question_ops.print_questions_to_user(user_ops.current_user);
			if (n == 2)
				question_ops.print_questions_from_user(user_ops.current_user);
			if (n == 3)
			{ question_ops.answer_question(user_ops.current_user);
				question_ops.update_database(); }
			if (n == 4)
			{ question_ops.delete_question(user_ops.current_user);
				question_ops.fill_user_questions(user_ops.current_user);
				question_ops.update_database();}
			if (n == 5)
			{
				auto to_user = user_ops.read_user_id();
				if (to_user.first != -1)
				{
					question_ops.ask_question(user_ops.current_user,to_user);
					question_ops.update_database();
				}
			}
			if (n == 6)
				user_ops.list_users_names_ids();
			if (n == 7)
				question_ops.list_feed();
			if (n == 8)
				return;
		}
	}
};
int main()
{
	ask_system project;
	project.run();
}