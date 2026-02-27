#define MAX_EXPRESS 500	/* size of the expression string */
#define MAX_SEMINAME 128 /* Max length of semiconductor names */
#define MAX_MESSAGE 600 /* Max length of message string */
#define MAX_FILENAME 1024 /* Max length of filenames */
#define MAX_EXTENSION 30 /* Max length of filename extensions */
#define MAX_KEYWORD 128 /* Max length of keywords */
#define MAX_IMPLANTS 10 /* Max number of implants */
#define MAX_INPUTLINE 1024 /* Max length of input line */
#define SCHOTTKY 0 //contact type for Schottky
#define OHMIC 1 //contact type for Ohmic
#define SLOPE 2 //contact type for slope BC
#define CHARGE 3 //contact type for charge BC
#define SurfContact -1 //contact number for surface contact
#define SubContact -2 //contact number for substrate contact
#define NoVoltageSource -9999 //contact number for substrate contact

struct mesh_type{
    //int contact;	/* contact definition for each mesh*/
    int noelec;	/* set if no electrons are allowed at this mesh point*/
    int noholes;	/* set if no holes are allowed at this mesh point*/
    int fullyionized;	/* set if mesh point's dopants are fully ionized*/
    double meshcoord;	/* real space coordinate of each mesh point*/
    double v;		/*conduc. band energy without offsets*/
    double Eg;	/*energy gap for this mesh point*/
    double offset;	/* band offset for this mesh point*/
    double dy;	/*delta y in Angstroms*/
    double eem;	/*effective electron mass for this mesh point*/
    double valley;	/* Cond band valley degeneracy for this mesh point*/
    double ehm;	/*effective heavy hole mass for this mesh point*/
    double elhm;	/* effective light hole mass for this mesh point*/
    double esohm;	// effective split-off hole mass for this mesh point
    double Eso;	// split-off band energy for this mesh point relative to valence band edge
    double Nc;	/*conduction band effective density of states*/
    double Nv;	/*valence band effective density of states*/
    double Nvso;	//split-off band effective density of states
    double Nd;	/*donor concentration*/
    double Ndd;	/*deep donor concentration*/
    double Nds;	/*Sheet donor concentration, cm-2/eV*/
    double Na;	/*acceptor concentration*/
    double Nda;	/*deep acceptor concentration*/
    double Nas;	/*Sheet acceptor concentration, cm-2/eV*/
    double esemi;	/* semiconductor relative dielectric constant*/
    double Ed;	/*donor ionization energy*/
    double Edd;	/*deep donor ionization energy*/
    double Ea;	/*acceptor ionization energy*/
    double Eda;	/*deep acceptor ionization energy*/
    double emob;	/* electron mobility*/
    double hmob;	/*  hole mobility*/
    double taup;	/* hole recombination time*/
    double taun;	/* electron recombination time*/
    double polariz;	/* polarization */
    double alpha;	/* absorption coefficient*/
    double Edst;	/* Sheet donor ionization, Energy of top of band*/
    double Edsb;	/* Sheet donor ionization, Energy of bottom of band*/
    double East;	/* Sheet acceptor ionization, Energy of top of band*/
    double Easb;	/* Sheet acceptor ionization, Energy of bottom of band*/
    double Efn;	/*Fermi Level for electrons*/
    double Efp;	/* Fermi level for holes*/
    double fcharge;	/* fixed charge array*/
    double polcharge; /* Polarization charge */
    double n;		/*electron concentration*/
    double p;		/*hole concentration*/
    double deriv;	/* partial derivative of charge concentration*/
    double c;	/* workspace for adjust routine */
    double d;	/* workspace for adjust routine */
    double e;	/* workspace for adjust routine */
    double f;	/* workspace for adjust routine */
    double aBohrElec;	/* Electron Bohr radius */
    double aBohrHhole;	/* Heavy hole Bohr radius */
    double aBohrLhole;	/* Light hole Bohr radius */
    struct contact_type *pContact; //pointer to contact for this point
    struct layer_type *pLayer;		/* layer pointer for this mesh point */
    struct mesh_type *pUpMesh;	/* Pointer to mesh point above*/
    struct mesh_type *pDownMesh;	/* Pointer to mesh point below*/
};

struct param_type{ //data structure for reading parameter file
    double depth; //depth of the point
    double Eg;	/*energy gap for this mesh point*/
    double offset;	/* band offset for this mesh point*/
    double esemi;	/* semiconductor relative dielectric constant*/
    double eem;	/*effective electron mass for this mesh point*/
    double valley;	/* Cond band valley degeneracy for this mesh point*/
    double ehm;	/*effective heavy hole mass for this mesh point*/
    double elhm;	/* effective light hole mass for this mesh point*/
    double esohm;	// effective split-off hole mass for this mesh point
    double Eso;	// split-off band energy for this mesh point relative to valence band edge
    double Nd;	/*donor concentration*/
    double Ndd;	/*deep donor concentration*/
    double Na;	/*acceptor concentration*/
    double Nda;	/*deep acceptor concentration*/
    double Ed;	/*donor ionization energy*/
    double Edd;	/*deep donor ionization energy*/
    double Ea;	/*acceptor ionization energy*/
    double Eda;	/*deep acceptor ionization energy*/
    double emob;	/* electron mobility*/
    double hmob;	/*  hole mobility*/
    double taun;	/* electron recombination time*/
    double taup;	/* hole recombination time*/
    double polariz;	/* polarization */
    double alpha;	/* absorption coefficient*/
    double n;	// electron concentration
    double p;	// hole concentration
};
	
struct layer_type{
	double dyLayer;		/* dy for each layer (in Angstroms)*/
    int mtrlType;       // Identifier for the type of material, 0=3D, 1=2D, 2=graphene
	char EfLayer[MAX_EXPRESS];		/* Fermi level for a layer*/
	char NaLayer[MAX_EXPRESS];		/* layer acceptor concentration*/
	char EaLayer[MAX_EXPRESS];		/* layer acceptor ionization level*/
	char NdLayer[MAX_EXPRESS];		/* layer donor concentration*/
	char EdLayer[MAX_EXPRESS];		/* layer donor ionization level*/
	char NdaLayer[MAX_EXPRESS];	/* layer deep acceptor concentration*/
	char EdaLayer[MAX_EXPRESS];	/* layer deep acceptor ionization level*/
	char NddLayer[MAX_EXPRESS];	/* layer deep donor concentration*/
	char EddLayer[MAX_EXPRESS];	/* layer deep donor ionization level*/
	char NdsLayer[MAX_EXPRESS];	/* layer sheet donor concentration*/
	char EdsLayer[MAX_EXPRESS];	/* layer sheet donor ionization level*/
	char NasLayer[MAX_EXPRESS];	/* layer sheet acceptor concentration*/
	char EasLayer[MAX_EXPRESS];	/* layer sheet acceptor ionization level*/
	double sheet;				/* Sheet charge at the top/bottom of layer*/
	double tLayer;				/* thickness of layer*/
	char xvalLayer[MAX_EXPRESS];	/* Layer semiconductor x value*/
	char yvalLayer[MAX_EXPRESS];	/* Layer semiconductor y value*/
	char zvalLayer[MAX_EXPRESS];	/* Layer semiconductor z value*/
	char wvalLayer[MAX_EXPRESS];	/* Layer semiconductor w value*/
	char EgLayer[MAX_EXPRESS];		/* Layer band gap*/
	char offsetLayer[MAX_EXPRESS];	/* Layer delta Ec*/
	char erLayer[MAX_EXPRESS];		/* Layer dielectric constant*/
	char eemLayer[MAX_EXPRESS];		/* Layer electron effective mass*/
	char mhhLayer[MAX_EXPRESS];		/* Layer heavy hole effective mass*/
	char mlhLayer[MAX_EXPRESS];		/* Layer light hole effective mass*/
	char msohLayer[MAX_EXPRESS];	// Layer split-off hole effective mass
	char EsoLayer[MAX_EXPRESS];		// Layer split-off band energy
	char emobLayer[MAX_EXPRESS];	/* Layer electron mobility*/
	char hmobLayer[MAX_EXPRESS];	/* Layer hole mobility*/
	char taunLayer[MAX_EXPRESS];	/* Layer electron lifetime*/
	char taupLayer[MAX_EXPRESS];	/* Layer hole lifetime*/
	char polarizLayer[MAX_EXPRESS];	/* Layer polarization*/
	char alphaLayer[MAX_EXPRESS];	/* Layer absorption coefficient*/
	char valleyLayer[MAX_EXPRESS];	/* Layer valley degeneracy*/
	double nsLayer;		/* Layer electron sheet concentration*/
	double psLayer;		/* Layer hole sheet concentration*/
	char nameLayer[MAX_SEMINAME];  /* name for each layer*/
	//char contactLayer[MAX_VOLTSOURCES];	/* Layer contact specification*/
	int numberLayer;	/* Layer counter*/
	int noholesLayer;	/* Layer no holes flag*/
	int noelecLayer;	/* Layer no electrons flag*/
	int fullyionLayer;	/* Layer fully ionized flag*/
	int sheetUp;		/* Flag.  True if sheet charge is at the top of layer */
	struct layer_type *pUpLayer;	/* pointer to layer above*/
	struct layer_type *pDownLayer;	/* pointer to layer below*/
};
	
struct dummy_layer_type{
	double dyLayer;		/* dy for each layer (in Angstroms)*/
    int mtrlType;       // Identifier for the type of material
	double EfLayer;		/* Fermi level for a layer*/
	double NaLayer;		/* layer acceptor concentration*/
	double EaLayer;		/* layer acceptor ionization level*/
	double NdLayer;		/* layer donor concentration*/
	double EdLayer;		/* layer donor ionization level*/
	double NdaLayer;	/* layer deep acceptor concentration*/
	double EdaLayer;	/* layer deep acceptor ionization level*/
	double NddLayer;	/* layer deep donor concentration*/
	double EddLayer;	/* layer deep donor ionization level*/
	double NdsLayer;	/* layer sheet donor concentration*/
	double EdsLayer;	/* layer sheet donor ionization level*/
	double NasLayer;	/* layer sheet acceptor concentration*/
	double EasLayer;	/* layer sheet acceptor ionization level*/
	double sheet;		/* Sheet charge at the top/bottom of layer*/
	double tLayer;		/* thickness of layer*/
	double xvalLayer;	/* Layer semiconductor x value*/
	double yvalLayer;	/* Layer semiconductor y value*/
	double zvalLayer;	/* Layer semiconductor z value*/
	double wvalLayer;	/* Layer semiconductor w value*/
	double EgLayer;		/* Layer band gap*/
	double offsetLayer;	/* Layer delta Ec*/
	double erLayer;		/* Layer dielectric constant*/
	double eemLayer;	/* Layer electron effective mass*/
	double mhhLayer;	/* Layer heavy hole effective mass*/
	double mlhLayer;	/* Layer light hole effective mass*/
	double msohLayer;	// Layer split-off hole effective mass
	double EsoLayer;	// Layer split-off band energy
	double emobLayer;	/* Layer electron mobility*/
	double hmobLayer;	/* Layer hole mobility*/
	double taunLayer;	/* Layer electron lifetime*/
	double taupLayer;	/* Layer hole lifetime*/
	double polarizLayer;/* Layer polarization*/
	double alphaLayer;	/* Layer absorption coefficient*/
	double valleyLayer;	/* Layer valley degeneracy*/
	double nsLayer;		/* Layer electron sheet concentration*/
	double psLayer;		/* Layer hole sheet concentration*/
	char nameLayer[MAX_SEMINAME];  /* name for each layer*/
	//char contactLayer[MAX_VOLTSOURCES];	/* Layer contact specification*/
	int numberLayer;	/* Layer counter*/
	int noholesLayer;	/* Layer no holes flag*/
	int noelecLayer;	/* Layer no electrons flag*/
	int fullyionLayer;	/* Layer fully ionized flag*/
	int sheetUp;		/* Flag.  True if sheet charge is at the top of layer */
	struct layer_type *pUpLayer;	/* pointer to layer above*/
	struct layer_type *pDownLayer;	/* pointer to layer below*/
};
	
struct mtrl_line_type { //structure for each interval in the materials structure
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
	double wmin;
	double wmax;
	char expression[MAX_EXPRESS];
	struct mtrl_line_type *pNextInterval;
};
	
struct mtrl_type {
	double Nd;
	double Na;
	double Ndd;
	double Nda;
	struct mtrl_line_type *Egline;
	struct mtrl_line_type *offsetline;
	struct mtrl_line_type *erline;
	/*struct mtrl_line_type *emtline;
	struct mtrl_line_type *emlline;
	struct mtrl_line_type *valleyline;
	struct mtrl_line_type *emin1line;
	struct mtrl_line_type *emc1lline;
	struct mtrl_line_type *valley1line;
	struct mtrl_line_type *ein2line;
	struct mtrl_line_type *ec2line;
	struct mtrl_line_type *valley2line;*/
	struct mtrl_line_type *eemline;
	struct mtrl_line_type *valleyline;
	struct mtrl_line_type *mhhline;
	struct mtrl_line_type *mlhline;
	struct mtrl_line_type *msohline;
	struct mtrl_line_type *Esoline;
	struct mtrl_line_type *Edline;
	struct mtrl_line_type *Ealine;
	struct mtrl_line_type *Eddline;
	struct mtrl_line_type *Edaline;
	struct mtrl_line_type *emobline;
	struct mtrl_line_type *hmobline;
	struct mtrl_line_type *taunline;
	struct mtrl_line_type *taupline;
	struct mtrl_line_type *polarizline;
	struct mtrl_line_type *alphaline;
    double thickness;
};

// Structure for list of available materials.  Points to parameters list
struct mtrl_list {
	char name[MAX_SEMINAME];
	char type[20];
	char family[MAX_SEMINAME];
	char comment[MAX_MESSAGE];
	struct mtrl_type *pMaterial;
	struct mtrl_list *pNext_mtrl;
};

// Structure for materials data.  Passed into globals for a simulation run
struct mtrl_data_type {
	char *matrlfilePath;	// materials file path name
	char *MatrlFile;     //Contents of the materials file
	struct mtrl_list *pTopMtrl;	// pointer to top material list
};

// Structure for storing the multiruns data
struct multiRun_type {
    struct contact_type *pContact;
    int multiCountMax;
    int multiCount;
    char extension[MAX_EXTENSION];
};

// Structure for storing the voltage source data
struct voltageSource_type {
    int sourceNumber; //voltage source number
    double vbias; //Voltage value
    int multiRun; //Set if a voltage sweep is requested
	double vincrement;	// voltage increment for multiple runs
	double vstop;		// stop voltage for multiple runs
    struct voltageSource_type *pNextVoltageSource; //Next voltage source in list
};

// Structure for storing the contact / boundary condition data
struct contact_type {
    int contactNumber; //Number of the contact
    int contactType;  //Type of contact
    double contactParameter; //contact info,
    double realPosition; //position of contact in real space
    struct mesh_type *pMeshPosition; //Pointer to meshpoint of contact
    int voltageSourceNumber; //number of the attached voltage source
    struct voltageSource_type *pVoltageSource; //Pointer to voltage source used
    struct contact_type *pNextContact; //Next contact in list
};

// Structure for bound state search limits defined in input file
struct BndStBounds_type {
    int eBoundsSet;     //set if the electron search range has been set
    int hhBoundsSet;    //set if the heavy hole search range has been set
    int lhBoundsSet;    //set if the light hole search range has been set
    int sohBoundsSet;   //set if the split-off hole search range has been set
    double eUB;         //electron search range upper bound
    double eLB;         //electron search range upper bound
    double hhUB;        //heavy hole search range upper bound
    double hhLB;        //heavy search range upper bound
    double lhUB;        //light hole search range upper bound
    double lhLB;        //light search range upper bound
    double sohUB;       //split-off hole search range upper bound
    double sohLB;       //split-off search range upper bound
};

struct global_type {
	
	FILE *pFile[10];			/* pointer array to files */

	struct layer_type *pTopLayer;	/* pointer to top layer of input structure */
	struct mtrl_data_type *pMtrl_data;	// pointer to the materials data
	struct mesh_type *pTopMesh;	/* pointer to top mesh */
	struct mesh_type *pBottomMesh;	/* pointer to bottom mesh */
	struct mesh_type *pStartSim;	/* position to start simulation, 0 except in etching sim*/
	struct mesh_type *pStartMesh;	/* starting position of active mesh points*/
	struct mesh_type *pStopMesh;	/* stoping position of active mesh points*/
	struct mesh_type *pSchStart;	/* starting point of schrodinger mesh*/
	struct mesh_type *pSchStop;	/* stoping point of schrodinger mesh*/
    struct multiRun_type multiRun; //data for multi run simulation
    struct contact_type *pFirstContact; //pointer to contact list
    struct voltageSource_type *pFirstVoltage; //pointer to voltage source list
    struct BndStBounds_type BndStBounds; //structure for bound states limits

	double defaultdy;        /* default dy in Ang */
	double realschstart;	/* user input schrodinger mesh starting point*/
	double realschstop;		/* user input schrodinger mesh stopping point*/
	double temperature;		/* system temperature*/
	double kT;				/* k times temperature*/
	double ConvgError;		/* Convergence error register*/
	double poierr;			/* maximum error in the poisson equation*/
	double *pPotential;		/* potential for schrodinger solution*/
	double *pMass;			/* partical mass for schrodinger solution*/
	double *pEvcharge;		/* charge from the schrodinger solution*/	
	double *pEvderiv;		/* derivative from the schrodinger solution*/
	double *pSdy;			/* delta y for schrodinger*/
	double *pEval;			/* eigen values found (light holes when finished)*/
	double *pEleceval;		/* electron eigenvalues*/
	double *pHheval;		/* heavy hole eigenvalues*/
	double *pLheval;		/* light hole eigenvalues*/
	double *pSoheval;		// split off hole eigenvalues
	double **pEigenv;		/* eigen vectors found*/
	double **pElecevec;		/* electron eigenvectors*/
	double **pHhevec;		/* heavy hole eigenvectors*/
	double **pLhevec;		/* light hole eigenvectors*/
	double **pSohevec;		/* split off hole eigenvectors*/
	double eLB;				// elec lower eigenvalue bound
    double eUB;				// elec upper eigenvalue bound
    double hhLB;				// hole lower eigenvalue bound
    double hhUB;				// hole upper eigenvalue bound
    double lhLB;				// hole lower eigenvalue bound
    double lhUB;				// hole upper eigenvalue bound
	double sohLB;			// split off lower eigenvalue bound
	double sohUB;			// split off upper eigenvalue bound
	double vmulti;			/* voltage at each step for multiple runs*/
	double cvvoltage;		/* voltage used in CV calculation*/
	double capacitance;		/* capacitance calculated in CV*/
	double dose[MAX_IMPLANTS];	/* ion implant dose*/
	double Rp[MAX_IMPLANTS];	/* ion implant projected range*/
	double dRp[MAX_IMPLANTS];	/* ion implant standard deviation*/
	double etchdepth;		/* depth to which an etching simulation is carried*/
	double etchinc;			/* depth increment in etching simulation*/
	double converg;			/* convergence criteria*/
	double CVconverg;		/* CV convergence criteria*/
	double crossover;		/* band correction at crossover to Schrodinger solution*/
	double y;				// y coordinate */
	double y_loc;			// y coordinate 0 to 1 for each layer
	double xval;			// x value to be used in calculations
	double yval;			// y value to be used in calculations
	double wval;			// w value to be used in calculations
	double zval;			// z value to be used in calculations
    double epsilonA;         //used for field dep dielectric constant
    double epsilonB;        //used for field dep dielectric constant
	       
	int errorCode;          // code for error message
    int iterationCount;     // Iteration count of simulation
    long int order;			// order of schrodinger matrix
    int numMesh;            // Number of mesh points
	int numeval;			/* number of eigenvalues found (light holes when finished)*/	
	int numeleceval;		/* number of electron eigenvalues*/
	int numhheval;			/* number of heavy hole eigenvalues found*/
	int numlheval;			/* number of light hole eigenvalues found*/
	int numsoheval;			/* number of split-off hole eigenvalues found*/
	int impnumber;			/* number of implants*/
	int maxcount;			/* max number of iterations*/
    int numLayers;
    
	char *InputFile;     //Contents of the input file
	char *OutputFile;		//Contents of output file
	char *StatusFile;		//Contents of status file
	char *CVFile;			//Contents of CV file
	char *ShellFile;		//Contents of shell file
	char *inputPath;		// path to input file
	char *filename;		// input file name
	char imptype[MAX_IMPLANTS];			// implant type, a for acceptor, d for donor
};

struct flag_type {
	// logical flags as integers 
	int stopthemusic;			/* set if the calculation should be aborted*/
	int schrodinger;			/* set if a schrodinger solution is to be done*/
	int schstartflag;			/* set if schstart has been defined*/
	int schstopflag;			/* set if schstop has been defined*/
	int badversion;				/* set if the wrong version of the material file is found*/
	int noinputfile;			/* set if no input file has been selected*/
	int schex;					/* set when the schrodinger solution begins*/
	int outfileexists;			/* set if an output file exists*/
	int eigenfail;				/* set when eigen solver has failed*/
	int multiruns;				/* set if there is more than one voltage value to be run*/
	int etched;					/* set if structure is simulated as it etches*/
	int etchonly;				/* set if only the sheet resistance is wanted, no band diagram*/
	int firstetch;				/* set on the first iteration of a etch simulation*/
    int invert;                 // Find a inverse solution: fixed charge given a mobile charge distribution.
	int cv;						/* set if a CV run is requested*/
	int firstcv;				/* set if the first point of a CV run*/
	int cvonly;					/* set if only CV output is wanted*/
	int firstlayer;				/* set until the first layer is read*/
	int checkfamily;			/* set during input file read. Check all layers of same family*/
	int implant;				/* set if an implant is requested*/
	int exchange;				/* set if charge interchange is used in hamiltonian*/
	int defaultNoHoles;			/* set if no holes are wanted for entire simulation*/
	int defaultNoElec;			/* set if no electrons are wanted for entire simulation*/
	int defFullyIonized;		/* sef if dopants in entire structure are ionized*/
	int DangerWill;				/* set if insufficient confinement in schodinger mesh*/
	int printDanger;			/* set if "Danger Will Robinson" is to be printed (first time only)*/
	int stopDanger;				/* set if simulation stops on "Danger Will Rob." error*/	
	int ignoreDanger;			/* set if simulation ignores "Danger Will Rob." error*/
	int cleanFiles;				/* set if old output files are to be deleted (this is the default)*/
	int restart;				/* set if simulation is to restart using existing output*/
	int noStatus;				/* set if the user does not want a status file*/
	int firstSim;				/* set if this is first simulation, used for graph wind. close decision*/ 
	int readParam;				/* set to read parameter file*/
	int findBst;				// set to find boundstates
    int BndStRangeSet;          // set if the user has defined the limits in finding bounstates    int
	int useBstDefaults;			// set to use the default limits in finding boundstates
	int silent;					/* set if run data should be sent to a file instead of to the console */
	int schMemAlloc;			/* set when schrodinger mememory has been allocated */
	int schElecMemAlloc;		/* set when memory allocated for elec eigensystem */
	int schHhMemAlloc;			/* set when memory allocated for heavy hole eigensystem */
	int schLhMemAlloc;			/* set when memory allocated for light hole eigensystem */
	int schSohMemAlloc;			/* set when memory allocated for Split-off hole eigensystem */
};

