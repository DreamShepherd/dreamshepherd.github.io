// Definitions and functions used in 1D Poisson

// Program compile type definitions */
#define NORMAL 1
#define COMMAND_LINE 2
#define program_type NORMAL

// Define Platform, Mac or PC
#define MAC 1
#define PC 2
#define PLATFORM PC

// Define Materials version
#define MaterialsVersion "beta7"

// default path
//#define PATH "/Users/gsnider/Programming/1D Poisson/source files/C files/1D Poisson/Previous Releases/Beta 9/Beta 9b2/" // Define default input file path
#define PATH "/Applications/1D Poisson/" // Define default materials file path


// Token definitions
#define true 1
#define false 0
#define caseSens 1
#define caseInSens 0
#define q 1.60218e-19		/* electron charge*/
#define dicon 8.85418e-14	/* free space dielectric constant*/
#define invh 1.0e16		/* mesh space multiplier */
#define MAXOP 100	/* max size of operand or operator or layer loop stack */
#define NUMBER '0'	/* signal that a number was found */
#define threeDmtrl 0  // type identifier for 3 D materials
#define twoDmtrl 1  // type identifier for 3 D materials
#define graphenemtrl 2  // type identifier for 3 D materials
#define FermiVel 1e8 //Fermi velocity in graphene
#define FindBounds 0 //Find bounds in Schrodinger calculation
#define FixBounds 1 //Use fixed bounds in Schrodinger calculation


/* Function prototypes */
int Init_1DPoisson(struct global_type *pGlobals, struct flag_type *pFlags);
int fieldtrim(char info[], int *startsearch,char field[],char subfield[],int keepCase);
char *fileread(char filename[], char access[], char extension[]);
int fileopen(int unit, char name[], char access[], char extension[], struct global_type *pGlobals);
int readlayers(char **info, int *filepointer, struct global_type *pGlobals, struct flag_type *pFlags);
void fileclose(int unit, struct global_type *pGlobals);
int getInput(int unit, char info[], struct global_type *pGlobals);
int getInputNew(char file[], int *filepointer, char **info);
void alertbox(char messag[]);
int input(struct global_type *pGlobals, struct flag_type *pFlags);
int initialize(struct global_type *pGlobals, struct flag_type *pFlags);
double numconv(char field[]);
void lowercase(char input[]);
int currentfile(struct global_type *pGlobals, struct flag_type *pFlags);
void choose(struct global_type *pGlobals, struct flag_type *pFlags);
//int startPoisson(struct global_type *pGlobals, struct flag_type *pFlags);
int MultiRun(struct global_type *pGlobals, struct flag_type *pFlags);
void selrun(struct global_type *pGlobals, struct flag_type *pFlags);
void script(struct global_type *pGlobals, struct flag_type *pFlags);
int checkMaterials(struct mtrl_data_type *pMtrl_data);
int loadMaterials(struct mtrl_data_type *pMtrl_data);
struct mesh_type  *ymesh(double position,struct global_type *pGlobals);
int output(struct global_type *pGlobals, struct flag_type *pFlags);
void cvout(char filename[],double initcharge);
int onedpoisson(struct global_type *pGlobals, struct flag_type *pFlags);
void cleanup(void);
int boundstates(struct global_type *pGlobals, struct flag_type *pFlags);
void bndStDefaults(struct global_type *pGlobals, struct flag_type *pFlags);
int writestatus(struct global_type *pGlobals, struct flag_type *pFlags);
double cvcharge(struct global_type *pGlobals);
void charge(struct global_type *pGlobals, struct flag_type *pFlags);
void adjust(struct global_type *pGlobals, struct flag_type *pFlags);
void sheetcharge(double *nsTotal, double *psTotal, double *sheetres, struct global_type *pGlobals);
void mobilecharge(struct mesh_type *pMesh, double kT);
void fixedcharge(struct mesh_type *pMesh, double kT);
double electrons(struct mesh_type *pMesh, double kT);
double holes(struct mesh_type *pMesh, double kT);
double electronderiv(struct mesh_type *pMesh, double kT);
double holederiv(struct mesh_type *pMesh, double kT);
int schrodingersolve(struct global_type *pGlobals, struct flag_type *pFlags, int fixedBounds);
void listmaterials(struct global_type *pGlobals, struct flag_type *pFlags);
void materialparams(struct global_type *pGlobals, struct flag_type *pFlags);
int meshInit (struct mesh_type *pMesh, struct layer_type *pLayer, struct dummy_layer_type *pDefaultLayer,
		struct global_type *pGlobals, struct flag_type *pFlags);
double evaluate(char[],struct global_type *pGlobals, struct flag_type *pFlags);
int semisearch(char seminame[],double xval,double yval,double zval,double wval,
		 struct dummy_layer_type *pLayer, char simfamily[], struct global_type *pGlobals,
		 struct flag_type *pFlags);
int lin_grade(char expression[]);
int infix_postfix(char expression[]);
int getop(char s[], char input[], int *j, int *sign_possible);
int getSingField(char s[], char input[], int *j);
int processContacts(struct global_type *pGlobals, struct flag_type *pFlags);
int readParamFile(struct global_type *pGlobals, struct flag_type *pFlags);
double initBands(struct mesh_type *pMesh, double posCharge, double negCharge, double kT);
void invert(struct global_type *pGlobals, struct flag_type *pFlags);



