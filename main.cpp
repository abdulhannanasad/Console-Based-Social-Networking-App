#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class String;
class Controller;
class User;
class Page;
class Object;
class Post;
class Comment;
class Memory;
class Post_Content;
class Date;
class Activity;


class String
{

public:

	//Function for String Length
	static int strlen(const char* string)
	{
		int len = 0;
		for (int i = 0; string[i] != '\0'; i++)
		{
			len++;
		}
		return len;
	}


	//Function for String Copy
	static void strcopy(char*& string1, char*& string2)
	{
		int count = strlen(string2);
		string1 = new char[count + 1];
		int i = 0;
		while (i < count)
		{
			string1[i] = string2[i];
			i++;
		}
		string1[i] = '\0';
	}


	// Function to get string from buffer
	static char* get_from_buffer(char* string)
	{
		char* newString;
		strcopy(newString, string);
		return newString;
	}

	//Function for String Comparison
	static bool str_comp(char* string1, const char* string2)
	{
		int count1 = strlen(string1);
		int count2 = strlen(string2);
		if (count1 == count2)
		{
			for (int i = 0; i < count1; i++)
			{
				if (string1[i] != string2[i])
				{
					return false;
				}
			}
		}

		else
		{
			return false;
		}
		return true;
	}
};


class Controller
{
private:
	User** users;
	Page** pages;
	Post** post;
	static int total_comments;
	static int total_users;
	static int total_pages;
	static int total_posts;
	static int total_activities;
public:
	void load_data(char* file1, char* file2, char* file3, char* file4)
	{
		read_pages(file2);
		read_users(file1);
		read_posts(file3);
		read_comments(file4);
	}


	void read_pages(char*);
	void read_users(char*);
	void link_users_and_pages(char*);
	void read_posts(char*);
	void read_comments(char*);
	void run();
	static void increase_post();
	Object* object_search(char*);
	User* user_search(char*);
	Post* post_search(char*);
	Page* page_search(char*);
	static int get_posts();
	void add_post(Memory*);
	void post_a_comment(Post*, char*, Object*);
	void like_post(Object*, Post*);
	void share_memory(Post*, char*, User*);
	~Controller();
};

int Controller::total_users = 0;
int Controller::total_comments = 0;
int Controller::total_pages = 0;
int Controller::total_posts = 0;


class Date
{
private:
	int day;
	int month;
	int year;
public:
	static int d;
	static int m;
	static int y;
	
	void read_date(ifstream& ifile)
	{
		ifile >> day;
		ifile >> month;
		ifile >> year;
	}

	static void set_current_date(int day, int month, int year);
	static void view_current_date();
	int getd();
	int getm();
	int gety();
	static int getday();
	static int getmonth();
	static int getyyear();
	void set_date(int d, int m, int y);
	void view_date();
};

int Date::d = 0;
int Date::m = 0;
int Date::y = 0;


class Object
{
protected:
	char* id;
	Post** timeline;
	int no_of_timeline;
public:
	virtual void display_id();
	virtual void display_name();
	virtual void view_friends();
	virtual void add_timeline(Post*);
	char* get_id();
	virtual ~Object();
};

class User :public Object
{
	char* fname;
	char* lname;
	User** friends_list;
	Page** liked_pages;
	int total_friends;
	int no_of_liked_pages;
	Memory* memo;
public:
	User();
	void read_user(ifstream&);
	void like_page(Page*);
	void add_timeline(Post*);
	void display_id();
	User* user_search(char*);
	void add_friend(User*);
	void display_name();
	void view_friends();
	void view_liked_pages();
	void view_last_48_hours();
	void view_home_page();
	void view_timeline();
	void view_memory();
	void share_memory(Memory*);
	~User();
};

class Post
{
protected:
	char* id;
	char* Text;
	int no_of_likes;
	int no_of_comments;
	Date SharedDate;
	Activity* activity;
	Object* shared_by;
	Object** liked_by;
	Comment** comments;
	Post_Content* content;
public:
	void read_posts(ifstream&);
	void set_shared_by(Object*);
	void add_liked_by(Object*);
	void display_id();
	bool date_comparison(Post*);
	void display_comments();
	void view_activity();
	void add_comment(Comment*);
	Post* post_search(char*);
	virtual void view_home();
	bool compare_current();
	void view_liked_list();
	void view_post();
	Object** get_liked_by();
	Comment** get_comments();
	int get_likes();
	int get_no_comments();
	Object* get_owner();
	int get_date();
	int get_month();
	int get_year();
	Post();
	~Post();
};

class Page :public Object
{
	char* title;
	int total_liked = 0;
public:
	void read_page(ifstream&);
	Page* page_search(char*);
	void display_id();
	void add_like();
	void add_timeline(Post*);
	void display_name();
	void view_last_48_hours();
	void view_page();
	~Page();
};

class Comment
{
	char* id;
	char* text;
	Object* comment_by;
public:
	void set_values(char*, Object*, char*);
	char* get_text();
	char* get_id_of_commenter();
	void display_comment();
	~Comment();
};

class Post_Content
{
public:
	virtual void display_activity() {}
	virtual ~Post_Content() {};
};

class Activity : public Post_Content
{
	int type;
	char* value;
public:
	void read_activity(ifstream&);
	void display_activity();
	~Activity();
};

class Memory :public Post
{
	Post* original_post;
public:
	Memory(Post*, char*);
	void view_home();
	~Memory();
};


// Start of Controller class function definitions
void Controller::read_users(char* file)
{
	ifstream ufile;
	ufile.open(file);
	if (ufile.is_open())
	{
		ufile >> total_users;
		users = new User * [total_users];
		for (int i = 0; i < total_users; i++)
		{
			users[i] = new User;
			users[i]->read_user(ufile);
		}
	}

	else
	{
		cout << "Sorry! File not found";
	}
	ufile.close();
}

void Controller::read_pages(char* file)
{
	ifstream pfile;
	pfile.open(file);
	if (pfile.is_open())
	{
		pfile >> total_pages;
		pages = new Page * [total_pages];
		for (int i = 0; i < total_pages; i++)
		{
			pages[i] = new Page;
			pages[i]->read_page(pfile);
		}
	}

	else
	{
		cout << "\a Sorry! File not found";
	}
	pfile.close();
}

void Controller::link_users_and_pages(char* file)
{
	char temp1[4] = { '\0' };
	char temp[4] = { '\0' };
	User* user = 0, * friends = 0;
	Page* page = 0;
	ifstream fin(file);
	fin >> temp1;
	while (String::str_comp(temp1, "-1") == 0)
	{
		user = user_search(temp1);
		fin >> temp;
		while (String::str_comp(temp, "-1") == 0)
		{
			friends = user_search(temp);
			user->add_friend(friends);
			fin >> temp;
		}
		fin >> temp;
		
		while (String::str_comp(temp, "-1") == 0)
		{
			page = page_search(temp);
			user->like_page(page);
			fin >> temp;
		}
		fin >> temp1;
	}
	fin.close();
}

void Controller::read_posts(char* file)
{
	ifstream pofile;
	pofile.open(file);
	if (pofile.is_open())
	{
		pofile >> total_posts;
		Object* temp = 0;
		char*** temp1 = new char** [total_posts];
		char tem[50];
		post = new Post * [total_posts];
		for (int i = 0; i < total_posts; i++)
		{
			int j = 0;
			post[i] = new Post;
			post[i]->read_posts(pofile);
			temp1[i] = new char* [10];
			for (int k = 0; k < 10; k++)
			{
				temp1[i][k] = 0;
			}

			pofile >> tem;
			temp = object_search(tem);
			post[i]->set_shared_by(temp);
			temp->add_timeline(post[i]);
			while (!pofile.eof())
			{
				pofile >> tem;
				if (tem[0] == '-')
				{
					break;
				}

				if (j < 10)
				{
					temp1[i][j] = String::get_from_buffer(tem);
					j++;
				}
			}
			for (int k = 0; k < 10; k++)
			{
				if (temp1[i][k] != 0)
				{
					temp = object_search(temp1[i][k]);
					post[i]->add_liked_by(temp);
				}
			}
		}

		for (int i = 0; i < total_pages; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				delete[] temp1[i][j];
			}
			delete[] temp1[i];
		}
		delete[] temp1;
	}

	else
	{
		cout << "\a Sorry! File not found";
	}
	pofile.close();
}

void Controller::read_comments(char* file)
{
	ifstream cfile;
	cfile.open(file);
	Comment** comments;
	if (cfile.is_open())
	{
		cfile >> total_comments;
		char temp[100];
		char* id, * text;
		comments = new Comment * [total_comments];
		for (int i = 0; i < total_comments; i++)
		{
			comments[i] = new Comment;
			cfile >> temp;
			id = String::get_from_buffer(temp);
			cfile >> temp;
			Post* postPtr = post_search(temp);
			postPtr->add_comment(comments[i]);
			cfile >> temp;
			Object* commentby = object_search(temp);
			cfile.getline(temp, 100, '\n');
			text = String::get_from_buffer(temp);
			comments[i]->set_values(id, commentby, text);
		}
	}

	else
	{
		cout << "\a Sorry! File not found";
	}
	cfile.close();
}

void Controller::run()
{
	User* temp;
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << "Command:\t Set current System date\n";
	Date::set_current_date(17, 4, 2024);
	Date::view_current_date();
	cout << "\n-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t Set current user \n";
	temp = user_search((char*)"u7");
	temp->display_name();
	cout << "successfully set as current user\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Friend List\n";
	temp->display_name();
	cout << "-Friends\n";
	temp->view_friends();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Commmand: View Liked Pages\n";
	temp->display_name();
	cout << "-Liked pages\n";
	temp->view_liked_pages();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View timeline\n";
	temp->display_name();
	cout << "-Timeline\n";
	temp->view_timeline();

	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Liked List(post5)\n";
	Post* temp1 = post_search((char*)"post5");
	temp1->view_liked_list();
	cout << " Command:\t Like Post (post5)\n";
	Object* n = temp;
	Post* p = post_search((char*)"post5");
	like_post(n, p);
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command: View Liked List (post5)\n";
	temp1->view_liked_list();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Page (p1)\n";
	Page* pt = page_search((char*)"p1");
	pt->view_page();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View home\n";
	//cout << "--- Abdullah Nadeem is Celebrating a birthday : \n 'Happy Birthday to my dear Mom' \n\t\tWaheed Khan : 'Happy Birthday Aunty'\t";
	//Date::view_current_date();
	cout << endl << endl;
	pt->view_page(); //Can comment this
	temp->view_home_page();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t Post Comment (post4,Good luck for your result)\n";
	temp1 = post_search((char*)"post4");
	post_a_comment(temp1, (char*)"Good luck for your result", temp);
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Post (post4)\n";
	temp1->view_home();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t Post Comment (post8,Thanks for the wishes)\n";
	temp1 = post_search((char*)"post8");
	post_a_comment(temp1, (char*)"Thanks for the wishes", temp);
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Post (post8)\n";
	temp1->view_home();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << "We hope you enjoy looking back at your memories on Facebook, from the most recent to those long ago!\n\n";
	cout << "On this day!\n";
	cout << " Command:\t See Your Memories\n";
	temp->view_memory();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t Share Memory (post10,\"Never thought i will be specialist field\")";
	p = post_search((char*)"post10");
	share_memory(p, (char*)"Never thought i will be specialist in this field", temp);
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Timeline\n";
	temp->view_timeline();
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t Set current user\n";
	temp = user_search((char*)"u11");
	temp->display_name();
	cout << " Successfully set as current user\n";
	cout << "-----------------------------------------------------------------------------------------------------------\n";
	//cout << " Command:\t View Home\n";
	//temp->view_home_page();
	//cout << "-----------------------------------------------------------------------------------------------------------\n";
	cout << " Command:\t View Timeline\n";
	temp->view_timeline();
}

void Controller::increase_post()
{
	total_posts++;
}

Object* Controller::object_search(char* string)
{
	Object* temp;
	if (string[0] == 'u')
	{
		for (int i = 0; i < total_users; i++)
		{
			temp = users[i]->user_search(string);
			if (temp != 0)
			{
				return temp;
			}
		}
	}
	
	else
	{
		for (int i = 0; i < total_pages; i++)
		{
			temp = pages[i]->page_search(string);
			if (temp != 0)
			{
				return temp;
			}
		}
	}
	return NULL;
}

User* Controller::user_search(char* string)
{
	User* temp;
	for (int i = 0; i < total_users; i++)
	{
		temp = users[i]->user_search(string);
		if (temp != 0)
		{
			return temp;
		}
	}
	return NULL;
}

Post* Controller::post_search(char* string)
{
	Post* temp;
	for (int i = 0; i < total_posts; i++)
	{
		temp = post[i]->post_search(string);
		if (temp != 0)
		{
			return temp;
		}
	}
	return NULL;
}

Page* Controller::page_search(char* string)
{
	Page* temp;
	for (int i = 0; i < total_pages; i++)
	{
		temp = pages[i]->page_search(string);
		if (temp != 0)
		{
			return temp;
		}
	}
	return NULL;
}


int Controller::get_posts()
{
	return total_posts;
}

void Controller::add_post(Memory* mem)
{
	Post** temp = new Post * [total_posts + 1];
	for (int i = 0; i < total_posts; i++)
	{
		temp[i] = post[i];
	}
	temp[total_posts] = mem;
	total_posts++;
	post = temp;
}

void Controller::post_a_comment(Post* p1, char* text, Object* obj)
{
	Comment* c = new Comment;
	total_comments++;
	p1->add_comment(c);
	c->set_values((char*)"post" + total_comments + 1, obj, text);
}

void Controller::like_post(Object* user, Post* post)
{
	post->add_liked_by(user);
}

void Controller::share_memory(Post* post, char* text, User* user)
{
	Memory* mem = new Memory(post, text);
	user->share_memory(mem);
	add_post(mem);
}

Controller::~Controller()
{
	for (int i = 0; i < total_users; i++)
	{
		delete users[i];
	}
	for (int i = 0; i < total_pages; i++)
	{
		delete pages[i];
	}
	for (int i = 0; i < total_posts; i++)
	{
		delete post[i];
	}
	delete[] users;
	delete[] pages;
	delete[] post;
}

// Start of User class function definitions
User::User()
{
	total_friends = 0;
	no_of_liked_pages = 0;
	fname = 0;
	lname = 0;
	liked_pages = 0;
	friends_list = 0;
	memo = 0;
}

void User::read_user(ifstream& ufile)
{
	char buffer[20] = { '\0' };

	ufile >> buffer;

	id = String::get_from_buffer(buffer);

	ufile >> buffer;
	fname = String::get_from_buffer(buffer);

	ufile >> buffer;
	lname = String::get_from_buffer(buffer);
}

void User::like_page(Page* temp)
{
	if (liked_pages == 0)
	{
		liked_pages = new Page * [10];
	}
	liked_pages[no_of_liked_pages] = temp;
	no_of_liked_pages++;
	temp->add_like();
}

void User::add_timeline(Post* obj)
{
	if (timeline == 0)
	{
		timeline = new Post * [10];
	}
	timeline[no_of_timeline] = obj;
	no_of_timeline++;
}

void User::display_id()
{
	cout << id;
}

User* User::user_search(char* temp)
{
	if (String::str_comp(temp, id))
	{
		return this;
	}
	else
	{
		return NULL;
	}
}

void User::add_friend(User* temp)
{
	if (friends_list == 0)
	{
		friends_list = new User * [10];
	}
	friends_list[total_friends] = temp;
	total_friends++;
}

void User::display_name()
{
	cout << fname << " " << lname << " ";
}

void User::view_friends()
{
	for (int i = 0; i < total_friends; i++)
	{

		cout << friends_list[i]->id << " - \t";
		friends_list[i]->display_name();
		cout << "\n";

	}
	cout << '\n';
}

void User::view_liked_pages()
{
	for (int i = 0; i < no_of_liked_pages; i++)
	{
		liked_pages[i]->display_id();
		cout << " - ";
		liked_pages[i]->display_name();
		cout << '\n';
	}
}

void User::view_last_48_hours()
{
	for (int i = 0; i < no_of_timeline; i++)
	{
		if (timeline[i]->compare_current())
		{
			timeline[i]->view_home();
		}
	}
}

void User::view_home_page()
{
	for (int i = 0; i < no_of_liked_pages; i++)
	{

		liked_pages[i]->view_last_48_hours();

	}
	
	for (int i = 0; i < total_friends; i++)
	{
		friends_list[i]->view_last_48_hours();

	}
}

void User::view_timeline()
{
	for (int i = 0; i < no_of_timeline; i++)
	{
		timeline[i]->view_home();
		cout << endl;
	}
}

void User::view_memory()
{
	for (int i = 0; i < no_of_timeline; i++)
	{
		int m = timeline[i]->get_month();
		int d = timeline[i]->get_date();
		int day = Date::getday();
		int month = Date::getmonth();
		if ((day == d) && (month == m))
		{
			timeline[i]->view_home();
		}
	}
}

void User::share_memory(Memory* memo)
{
	add_timeline(memo);
}

User::~User()
{
	delete[] fname;
	delete[] lname;
	delete[] friends_list;
	delete[] liked_pages;
	delete[] timeline;
}

//Start of Page class function definitions

void Page::read_page(ifstream& pfile)
{
	char temp[50];
	pfile >> temp;
	id = String::get_from_buffer(temp);
	pfile.getline(temp, 50, '\n');
	title = String::get_from_buffer(temp);
}

Page* Page::page_search(char* temp)
{
	if (String::str_comp(temp, id))
	{
		return this;
	}

	else
	{
		return NULL;
	}
}

void Page::display_id()
{
	cout << id;
}

void Page::add_like()
{
	total_liked++;
}

void Page::add_timeline(Post* obj)
{
	if (timeline == 0)
	{
		timeline = new Post * [10];
	}
	timeline[no_of_timeline] = obj;
	no_of_timeline++;
}

void Page::display_name()
{
	cout << title;
}

void Page::view_last_48_hours()
{
	for (int i = 0; i < no_of_timeline; i++)
	{
		if (timeline[i]->compare_current())
		{
			timeline[i]->view_home();
		}

	}
}

void Page::view_page()
{
	for (int i = 0; i < no_of_timeline; i++)
	{
		timeline[i]->view_home();
	}
}

Page::~Page()
{
	delete[] title;
	delete[] timeline;
}

// Start of Object class function definitions
void Object::display_id()
{
	cout << id << '\n';
}

void Object::display_name() {}

void Object::view_friends() {}

void Object::add_timeline(Post* p) {}

char* Object::get_id()
{
	return id;
}

Object::~Object()
{
	delete[] id;
}

//Start of Post class functiin definitions
Post::Post()
{
	no_of_likes = 0;
	no_of_comments = 0;
}

void Post::read_posts(ifstream& pofile)
{
	int tem;
	char temp[100];
	pofile >> tem;
	pofile >> temp;
	id = String::get_from_buffer(temp);
	SharedDate.read_date(pofile);
	pofile.ignore();
	pofile.getline(temp, 100, '\n');
	Text = String::get_from_buffer(temp);
	if (tem == 2)
	{
		activity = new Activity;
		activity->read_activity(pofile);
	}

	else
	{
		activity = 0;
	}
}

void Post::set_shared_by(Object* obj)
{
	shared_by = obj;
}

void Post::add_liked_by(Object* obj)
{
	if (liked_by == 0)
	{
		liked_by = new Object * [10];
	}
	liked_by[no_of_likes] = obj;
	no_of_likes++;
}

void Post::display_id()
{
	cout << id << "\t";
}

bool Post::date_comparison(Post* obj)
{
	if (SharedDate.gety() < obj->SharedDate.gety())
		return true;
	if (SharedDate.getm() < obj->SharedDate.getm())
		return true;
	if (SharedDate.getd() < obj->SharedDate.getd())
		return true;

	return false;
}

void Post::display_comments()
{
	for (int i = 0; i < no_of_comments; i++)
	{
		cout << "\t\t";
		comments[i]->display_comment();
		cout << endl;
	}
}

void Post::view_activity()
{
	cout << "--";
	shared_by->display_name();

	cout << " is ";
	activity->display_activity();

	cout << endl<<"\t" << Text << "\t";

	SharedDate.view_date();

	cout << endl;

}

void Post::add_comment(Comment* obj)
{
	if (comments == 0)
	{
		comments = new Comment * [10];
	}
	comments[no_of_comments] = obj;
	no_of_comments++;
}

Post* Post::post_search(char* temp)
{
	if (String::str_comp(temp, id))
	{
		return this;
	}
	else
	{
		return NULL;
	}
}

bool Post::compare_current()
{
	int d, m, y;
	d = Date::getday();
	m = Date::getmonth();
	y = Date::getyyear();
	if (SharedDate.gety() < y)
		return 0;
	if (SharedDate.getm() < m)
		return 0;
	if (SharedDate.getd() >= d - 2)
		return 1;

	return 0;
}

void Post::view_liked_list()
{
	for (int i = 0; i < no_of_likes; i++)
	{
		liked_by[i]->display_id();
		cout << " - ";
		liked_by[i]->display_name();
		cout << endl;
	}
}

void Post::view_home()
{
	cout << "--";
	shared_by->display_name();
	if (activity != 0)
	{
		cout << " is ";
		activity->display_activity();
	}
	SharedDate.view_date();
	cout << "\n\t" << Text << "\t";
	cout << endl;
	display_comments();
}

int Post::get_no_comments()
{
	return no_of_comments;
}

void Post::view_post()
{
	view_home();
}

int Post::get_likes()
{
	return no_of_likes;
}

int Post::get_date()
{
	return SharedDate.getd();
}

int Post::get_month()
{
	return SharedDate.getm();
}

int Post::get_year()
{
	return SharedDate.gety();
}

Object* Post::get_owner()
{
	return shared_by;
}

Object** Post::get_liked_by()
{
	return liked_by;
}

Comment** Post::get_comments()
{
	return comments;
}

Post::~Post()
{
	delete[] id;
	delete[] Text;
	delete activity;
	shared_by = 0;
	delete[] liked_by;
	delete[] comments;
}

// Start of Comment class function definitions
void Comment::set_values(char* id, Object* object, char* t)
{
	id = id;
	comment_by = object;
	text = t;
}

char* Comment::get_text()
{
	return text;
}

char* Comment::get_id_of_commenter()
{
	return comment_by->get_id();
}

void Comment::display_comment()
{
	comment_by->display_name();
	cout << ": " << text;
}

Comment::~Comment()
{
	delete[] id;
	delete[] text;
	comment_by = 0;
}

// Start of Memory class function definitions
Memory::Memory(Post* post, char* t)
{
	Text = String::get_from_buffer(t);
	shared_by = post->get_owner();
	Object** nliked_by = post->get_liked_by();
	Comment** ncomments = post->get_comments();
	no_of_comments = post->get_no_comments();
	no_of_likes = post->get_likes();
	liked_by = new Object * [no_of_likes];
	comments = new Comment * [no_of_comments];

	for (int i = 0; i < no_of_comments; i++)
	{
		comments[i] = ncomments[i];
	}

	for (int i = 0; i < no_of_likes; i++)
	{
		liked_by[i] = nliked_by[i];
	}

	int no = Controller::get_posts();
	id = String::get_from_buffer((char*)("post" + to_string(no + 1)).c_str());
	int d = Date::getday();
	int m = Date::getmonth();
	int y = Date::getyyear();
	activity = 0;
	SharedDate.set_date(d, m, y);
	original_post = post;
}

void Memory::view_home()
{
	cout << "--";
	shared_by->display_name();
	cout << " Shared a memory\n";
	cout << "\t";

	cout << "Shared " << Date::getyyear() - original_post->get_year() << " years ago";

	cout << "\t\n" << Text << "\t";

	cout << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	original_post->view_activity();
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

Memory::~Memory()
{
	delete[] Text;
	delete[] id;
}

// Start of Date Class function definitions
void Date::set_current_date(int day, int month, int year)
{
	d = day;
	m = month;
	y = year;
}

void Date::view_current_date()
{
	cout << "( " << d << '\\' << m << '\\' << y << " )";
}

int Date::getd()
{
	return day;
}

int Date::getm()
{
	return month;
}

int Date::gety()
{
	return year;
}

int Date::getday()
{
	return d;
}

int Date::getmonth()
{
	return m;
}

int Date::getyyear()
{
	return y;
}

void Date::set_date(int d, int m, int y)
{
	day = d;
	month = m;
	year = y;
}

void Date::view_date()
{
	if (d == day && m == month && y == year)
	{
		cout << "\t(1h)";
	}
	else 
		if (m == month && y == year)
		{
			int diff = d - day;
			if (diff <= 3)
			{
				cout << "\t( " << diff << "d)";
			}

		else
		{
			cout << "\t( " << day << '\\' << month << '\\' << year << " )";
		}
		}
	else 
		{
			cout << "\t( " << day << '\\' << month << '\\' << year << " )";
	}
}


//Start of Activity Class function definitions
void Activity::read_activity(ifstream& afile)
{
	char temp[50];
	afile >> type;
	afile.getline(temp, 50, '\n');
	value = String::get_from_buffer(temp);
}

void Activity::display_activity()
{
	if (type == 1)
	{
		cout << " feeling ";
	}
	else if (type == 2)
	{
		cout << " thinking about ";
	}
	else if (type == 3)
	{
		cout << " Making ";
	}
	else
	{
		cout << " Celebrating ";
	}
	cout << value;
}

Activity::~Activity()
{
	delete[] value;
}

int main()
{
	Controller mainApp;
	mainApp.load_data((char*)"Users.txt", (char*)"Pages.txt", (char*)"Posts.txt", (char*)"Comments.txt");
	mainApp.link_users_and_pages((char*)"Relations.txt");
	mainApp.run();
}