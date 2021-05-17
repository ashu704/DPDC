/* ----------------------------------------------------------------------------- 
 * new_pmu_or_pdc.h
 * ----------------------------------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

int add_PMU(char pmuid[], char ip[], char port[], char protocol[]);

void add_PMU_Node(struct Lower_Layer_Details *temp_pmu);

void* connect_pmu_tcp(void *);

void* connect_pmu_udp(void *);

int remove_Lower_Node(char pmuid[], char protocol[]);

void* remove_llnode(void*);

int put_data_transmission_off(char pmuid[], char protocol[]);

void* data_off_llnode(void* temp);

int put_data_transmission_on(char pmuid[], char protocol[]);

void* data_on_llnode(void* temp);

int configuration_request(char pmuid[], char protocol[]);

void* config_request(void* temp);

int add_PDC(char ip[], char protocol[]);

int remove_PDC(char ip[], char port_num[], char protocol[]);

void display_CT();

void create_command_frame(int type,int pmuid,char *);

int checkip(char ip[]);

/**************************************** End of File *******************************************************/
