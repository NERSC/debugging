//                 C++ and Threads demo
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <string.h>
void  user_templates(), trap_exception(), derived_class(), stl_view(), arrays(), pthreads_loop(), crash();int diveinall();
#include <stdio.h>
#include <iostream>
#include <pthread.h>


using namespace std;
static int ARRAYSIZE = 20;

long bb;
char * str;
long cc;


int do_parallel = 1;

int main(int argc, char **argv)
{

   str = (char *) malloc(100);
   strcpy( str, "Hello World" );

// General features
    arrays();
    diveinall();
    printf( "%s\n", str );

    long arr_longs[7] = {1,2,3,4,5,16,32};

    pthreads_loop();

// C++ features
    derived_class();
    stl_view();
    user_templates();
    trap_exception();

    exit(1);
}






//         ::: Exceptions

#define N 10
float my_array[N] = { 1.2, 2.3, 3.4, -.77, 5, 0, 7, 8, 9, 33};

void 
traverseArray(int n)
// you want to user individual next 
// commands here, otherwise the 
// application will run away
	{
	for (int i = 0; i < n; i++)
		{
		cout << "array[" << i << "] = " << my_array[i] << endl;
		if (my_array[i] < 0.0) throw i;
		}
	}


void
trap_exception()
	{
	void cleanup();
//
//  In single-step mode, we stop on exception
//
	try
		{ traverseArray(N); }
	catch(int e)
		{
		cout << "Exception on array element: " << e << endl;
		cleanup(); // or do some clean up here
		}
	cout << "No exceptions\n";
	}

void
cleanup()
	{
	exit(-1);
	}



class Base1 {
public:
	Base1(char c) {letter = c;}
	char getData() const { return letter;}
protected:
	char letter;
};

class Base2 {
public:
	Base2(int x) {value = x;}
	int getData() const { return value;}
        virtual int isDerived() { return 0; }
protected:
	int value;
};

class Base3 {
public:
	Base3(float f) {fvalue = f;}
	float getData() const { return fvalue;}
protected:
	float fvalue;
};

class Derived : public Base1, public Base2, public Base3 {
	friend ostream &operator<<(ostream &, const Derived &);
public:
	Derived(char, int, float, double);
	double getDouble() const;
        virtual int isDerived() { return 1; }
private:
	double dvalue;
};

Derived::Derived(char c, int i, float f, double d)
	: Base1(c), Base2(i), Base3(f)  { dvalue = d;}

double Derived:: getDouble() const { return dvalue;}

ostream & operator<<(ostream &output, const Derived &d)
	{
	output 
	       <<"\n character: " << d.letter
	       <<"\n integer: "<< d.value
				 <<"\n float number: " << d.fvalue
				 <<"\n double number: " << d.dvalue
				 <<"\n";
	return output;
	}



void derived_class()
	{
	Base1 b1('A'), *base1Ptr;
	Base2 b2(10), *base2Ptr,*base2Ptr2;
	Base3 b3(3.14159), *base3Ptr;
	Derived dd('Z', 7, 2.71, sqrt(2.0));
        
        

//
// Highlight the object + accessor methods for the 
// base class objects b1, b2 or b3 and select
// 'add to expression list'.
//
	cout <<"b1 has character " <<b1.getData()
			 <<"\nb2 has integer " << b2.getData()
			 <<"\nb3 has float value " << b3.getData()
			 <<"\nd contains " << dd ;

//
// Diving on the derived class object dd shows the class derivation hierarchy
// Dive on the parent or base classes. Changing the value of a member of the 
// base class also changes the value in the derived class
//
//
	cout <<"Accessing derived class members" 
			 <<"\n  character " << dd.Base1::getData()
			 <<"\n  integer   " << dd.Base2::getData()
			 <<"\n  float     " << dd.Base3::getData()
			 <<"\n  double    " << dd.getDouble()
                         <<"\n";

//
// You can also show that the qualified accessor methods and the 
// pointer notation below all work as expected within the expression 
// list window.
//

	cout <<"Look at Derived class object as a Base class object" << "\n";
	base1Ptr = &dd;
	cout <<"\n"<<"base1Ptr->getData() ==> " << base1Ptr->getData();
	base2Ptr = &dd;
	cout <<"\n"<<"base2Ptr->getData() ==> " << base2Ptr->getData();
	base3Ptr = &dd;
	cout <<"\n"<<"base3Ptr->getData() ==> " << base3Ptr->getData();
	cout <<"\n";

//
// You can use virtual functions in expression lists and
// even in the variable window. 
//
 
        base2Ptr2 = &b2;
	cout <<"\n"<<"base2Ptr->isDerived() ==> " << base2Ptr->isDerived();
	cout <<"\n"<<"base2Ptr2->isDerived() ==> " << base2Ptr2->isDerived();
	cout <<"\n";

	}


void stl_view()
	{	
	std::map<int,float> mf1;
	std::vector<int> vi1;
        std::list<Base1> lb1;
        std::string s1;
        s1="this is a string";
        Base1 b1('A');
        Base1 b2('B');
// BP here allows you to show how the
// list is shown growing as objects are
// added to it.
        lb1.push_back(b1); 
        lb1.push_back(b2); 
        lb1.push_back(b1);
        lb1.push_back(b2);
	for(int i=1;i<100;i*=2)
		{
		mf1[i]=(float)i*13.0;
		vi1.push_back((int)i);
                if(i<20)
			{
			lb1.push_back(b1);
			}
		else
			{
			lb1.push_back(b2);
			}
		cout << "i " << i << "\n" ;
		}
// BP here allows you to show
// the way TV shows STL strings
        s1="this is another string";
	cout << s1 << "\n";
	}

//   template class and template function

template <class T> void 
printArray(T *array, const int num)
	{
	for (int i = 0; i < num; i++)
		cout<< array[i] << " ";
	cout << "\n";
	}

template <class T>
class vect
	{
	public:
		vect(int = 0);
		vect(const vect &);
		vect(int, T *a);
		~vect();
		int getLen() const;
		const vect &operator = (const vect &);
		T  &operator [] (int);
		static int getNumArrays();
		int len;
		T *v;
	private:
		static int numArrays;
	};


template <class T>
int vect<T>::numArrays = 0;

template <class T>
int vect<T>::getNumArrays() {return numArrays;}

template <class T>
vect<T>::vect(int arrayLen)
	{
	numArrays++;
	len = arrayLen;
	v = new T[len];
	assert(v != 0);
	for (int i = 0; i < len; i++)
		v[i] = 0;
	}

template <class T>
vect<T>::vect(const vect &source)
	{
	numArrays++;
	len = source.len;
	v = new T[len];
	assert(v != 0);
	for (int i = 0; i < len; i++)
		v[i] = source.v[i];
	}

template <class T>
vect<T>::vect(int l, T *a)
	{
	numArrays++;
	len  = l;
	v = new T[len];
	assert(v != 0);
	for (int i = 0; i < len; i++)
		v[i] = a[i];
	}

template <class T>
vect<T>::~vect()	{--numArrays; delete [] v;}

template <class T>
int vect<T>::getLen() const {return len;}

template <class T>
T &vect<T>::operator[](int subscript)
	{
	assert((0 <= subscript)&& (len > subscript));
	return v[subscript];
	}

template <class T>
const vect<T> &vect<T>::operator=(const vect <T> &val)
	{
	if (&val != this)
		{
		delete [] v;
		len = val.len;
		v = new T[len];
		assert(v != 0);
		for (int i = 0; i < len; i++)
			v[i] = val.v[i];
		}
	return *this;
	}



//
// driver for template stuff 
//
void
user_templates()
	{
	float fa[7] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
	int ia[5] = {11,22,33,44,55};;
//
//  BP here allows you to step into constructors for vect <T>.
//
	vect<int>  i = vect<int>(5, ia);
	vect<float> d = vect<float>(7, fa);

	cout << "integer array ii : \n";
//
// BP here after the initalizers
//
	printArray(i.v, i.len);
	cout << "d : \n";
	printArray(d.v, d.len);

        cout << "integer length : " << i.getLen() << "\n";
        cout << "float length : " << d.getLen() << "\n";
        cout << "integer numarrays: " << i.getNumArrays() << "\n";
        cout << "float numarrays: " << d.getNumArrays() << "\n";
        for (int n=0; n< i.getLen(); n++) {
          cout << "element " << n << " has value " << i[n] << "\n";
        }

        
	}





#define PI 3.14152

// Our base class -  a few virtual functions - no pure virtual though

class Shape {
 public:
  virtual double area() const { return 0.0; }
  virtual double volume() const { return 0.0; }
  double m_area;
  double m_volume;
};

// A two dimension representation 

class Circle : public Shape{
public:
  Circle(const char *name, double radius);
  Circle(double radius);
  virtual ~Circle() {};
  virtual double area ();
  char * name () { return m_name; }
  char m_name[15];
  double  m_radius;

};

// A few overloaded constructors
Circle::Circle(const char *name, double radius) {
  strcpy(m_name, name);
  m_radius = radius;
  m_area = area();

}

Circle::Circle(double radius) {
  double myarea;
  strcpy(m_name, "Clevland");
  m_radius = radius;
  myarea = 2 * PI * m_radius * m_radius;
  m_area = area();

}

// Your basic circle area function

double Circle::area() {
  double result;
  result = PI * m_radius * m_radius;  // Our old friend, pi r squared...
  return result;
}

//  A Simple 3-D figure  - class exercise: Do a cone figure in the same
//  fashion.  - I forget how to calculate the surface area of a cone ;-) 
class Cylinder : public Circle {
public:
  Cylinder(const char *name, double radius, double height);
  Cylinder(double radius, double height);
  virtual ~Cylinder() {};
  virtual double area();
  virtual double volume();
  void resize(double radius, double height);
  double m_height;

};

// Similar overloaded constructors

Cylinder::Cylinder(const char *name, double radius, double height):
  Circle(name, radius) {
  strcpy(m_name, name);
  m_radius = radius;
  m_height = height;
  m_area = area();
  m_volume = volume();
}

Cylinder::Cylinder(double radius, double height) :
  Circle(radius) {
  strcpy(m_name, "Master");
  m_radius = radius;
  m_height = height;
  m_area = area();
  m_volume =  volume();
}

/* Surface area of a cylinder - both bases + the outer surface */
double Cylinder::area() {
  double result;
  result =  (2 * Circle::area() + 2*PI*m_radius*m_height);
  return (result);
}

// The volume of a cylinder -  Or is it?
double Cylinder::volume() {
  double result;
  result = (2 * Circle::area() * m_height);
  return (result);
}

// A method to resize the cylinder object instead of creating lots and 
// lots of cylinders 

void Cylinder::resize(double radius, double height) {
  m_radius = radius;
  m_height = height;
  m_area = area();
  m_volume = volume();
}

void arrays() {

  Shape  shape;
  Circle circle("circle", 4.0);
  Cylinder cylinder("cylinder", 4.2, 5);
  Cylinder cylly (10, 3);
  Circle *circe;
  double surf_vol [ARRAYSIZE][ARRAYSIZE];
  double vol [ARRAYSIZE][ARRAYSIZE];
  double start = 1.0;
  double step = .5;


  circe = new Circle ("circus", 42);
  cout << "circe = " << (void *)circe << "\n";
  //  cout << "*circe = " << (void)  *circe << "\n";
  cout << "&circe = " << (void *) &circe <<  "\n";

  for( int i = 0; i < ARRAYSIZE; i++ ) {
    for (int j = 0; j < ARRAYSIZE; j++) {
	  cylinder.resize(start + i*step, start + j*step);
	  surf_vol[i][j] = cylinder.volume()/cylinder.area();
	  vol[i][j] = cylinder.volume();
	}
  }
  // you can make the above array more interesting by
  // enabling an evaluation point that alters the behavior.
  // if (cylinder.volume() >=500) { step /= 2.0; 
  // cylinder.resize( start + i*step, start + j*step);
  // }
  



  cout << shape.area() << "\n";
  cout << circle.volume() << "\n";
  cout << cylinder.area() << "\n" ;
  cout << cylly.name() << "\n";
  cout << start << "\n";

}

    


typedef struct thread_args {
  float* a;
  float* b;
  int start;
  int end;
} thread_args;


#define NPERTHREAD 25
#define NUMTHREADS 4
#define NARRAY (NUMTHREADS*NPERTHREAD)



int clear_array(float* array){

int j;

  for (j=0;j<NARRAY;j++){
    array[j]=0.0;
  }
  return 1;
}

int simple_wave(float* array, float period, float amplitude){

int j;
float x;

  for (j=0;j<NARRAY;j++){
    x= (float) j / 10.0;
    array[j]=sin( x / period ) * amplitude;
  }
  return 1;
}

thread_args* initialize_args(float* a, float* b, thread_args* args){

thread_args* targ= (thread_args*) args;
int n;
for (n=0;n<NUMTHREADS;n++){
  targ[n].start=(0+n)*NPERTHREAD;
  targ[n].end=(1+n)*NPERTHREAD;
  targ[n].a=a;
  targ[n].b=b;
}
return targ;

}

void* combine_waves_worker(void* arguments){
  thread_args* arg= (thread_args*) arguments;
  int j; 
  float temp;
  for (j=arg->start;j<arg->end;j++){
    temp=arg->a[j]+arg->b[j];
    arg->a[j]=temp;
    temp=0.0;
  }

}


int parallel_combine_waves(float* a, float* b){

  pthread_t* threads = (pthread_t*) malloc(NUMTHREADS * sizeof(pthread_t));
  thread_args* args = (thread_args*) malloc(NUMTHREADS * sizeof(thread_args));
  void * thread_result;
  int i,status;


  args=initialize_args(a, b, args);

  for(i=0; i<NUMTHREADS; i++) {
    status = pthread_create(&threads[i], NULL, combine_waves_worker, (void *) &args[i]);
    if(status != 0)
      fprintf(stderr, "Create thread %d\n", i);
  }

  for(i=0; i<NUMTHREADS; i++) {
    status = pthread_join (threads[i], &thread_result);
    if(status != 0)
      fprintf(stderr, "Join thread %d\n", i);
  }


  free(args);
  free(threads);
  return 1;

}


int serial_combine_waves(float* a, float* b){

int j;
float temp;

  for (j=0;j<NARRAY;j++){
    temp=a[j]+b[j];
    a[j]=temp;
    temp=0.0;
  }
  return 1;
}

int combine_waves(float* a, float* b){

if (do_parallel)
  parallel_combine_waves(a,b);
else
  serial_combine_waves(a,b);
return 1;

}

void pthreads_loop(){

float compound[NARRAY];
float component[NARRAY];
int i;
float period;
float amplitude;



clear_array(component);
clear_array(compound);

for(i=1;i<20;i++){
  period=10.0/(float)i/2/3.14159;
  amplitude=400.0-(float)i*25.0;
  printf("period %f amplitude %f\n",period,amplitude);
  simple_wave(component,period,amplitude);
  combine_waves(compound,component);
}

}





typedef struct basic_t {
  int a;
  int b;
  float c;
} basic_t;

typedef struct compound_t {
  basic_t x;
  basic_t *y;
  float z;
} compound_t;



int diveinall() {

  int i;
  compound_t *array;

  array = (compound_t *) malloc(ARRAYSIZE * sizeof(compound_t));

  
  /* Fill in the array */
  for(i=0; i<ARRAYSIZE; i++) {

    array[i].x.a = i;
    array[i].x.b = 2*i;
    array[i].x.c = (float) 3*i;

    array[i].y = (basic_t *) malloc(sizeof(basic_t));
    array[i].y->a = i;
    array[i].y->b = 2*i;
    array[i].y->c = ((float) i) / 3.0;

    array[i].z = 100.0 * (float) i;
  }
  printf("end of dive in all\n");
  bb =  cc = 0xffffffffLL;
  ((int *)&bb)[1] = ((int *)&cc)[1] = 0xffffffffLL;
  return 1;
}
