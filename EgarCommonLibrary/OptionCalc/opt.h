#define ERR_VALUE 0

#define MAX_STEPS	303
#define PiF			.39894228
#define STEP		0.001
#define DaysPerYear 365.0
#define ONEDAY		1.0/DaysPerYear 
#define GL_STEPS	100
#define QSIZE       5
#define OPTIMIZED_BINOMIAL_STEPS    20

typedef double      C_QVECTOR[QSIZE];
typedef C_QVECTOR   C_QMATRIX[QSIZE];

double RateToCnt(double R, int Dte);
double NormalC(double s);

/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
double CalculateOption(	double	R,
						double	RF,
						double	S,
						double	K,
						double	V,
						int		Dte, 
                        bool	Call,
						bool	American,
						double* DivAmnt,
						double* DivYte,
						int		DivCount,
                        long    nSteps,
                        double* pSpotAjustDiv = NULL);

double CalculateOption_CRR(	double	R,
						    double	RF,
						    double	S,
						    double	K,
						    double	V,
						    int		Dte, 
                            bool	Call,
						    bool	American,
						    double* DivAmnt,
						    double* DivYte,
						    int		DivCount,
						    long	Steps,
                            C_QMATRIX* pQMatrix_Up,
                            C_QMATRIX* pQMatrix_Down,
                            double*    pSpotAjustDiv,
                            long*      pnModel);

double OP_Black( double	R, double S, double	K, double Vol, double T, double	RF, bool Call );

double OP_CoxRub( double R, double S, double K, double SIG, double T, double RF, bool CALL,
				 bool AMERICAN, int STEPS, int noofdiv,  double * DTime,double * DDIV,
                 C_QMATRIX* pQMatrix, double* pSpotAjustDiv);

double adjustVSKLog(double s, double k, double rd, double rf, 
				  double sigma, double t, double lam3, double lam4,
				  double OptVal);
double sigmaVSK( double rd, double rf, double k2);
double adjustVSK( double S0, double k, double rd, double rf, double t,
				double k2, double k3, double k4, double OptVal);

void FinDif(
	double s0,
	double k, 
	double lowB, 
	double upB, 
	double * rd,
	double * rf, 
	short numdiv, 
	double * tdiv, 
	double * div,
	short  volaNT, 
	short volaNX, 
	double * vola,   
	double * volaT, 
	double *volaX, 
	double voff,
	double texp, 
	short nt, 
	short nx, 
	short isCall, 
	short isEuro,
	short isTrin,
	double * optval, 
	double * delta, 
	double * gamma, 
	double * theta
);
