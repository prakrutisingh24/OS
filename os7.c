#include<stdio.h>
#include<stdint.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>

// virtual address space = 16-bits
//  page size is 1kB
// offset size = 10
// VPN size = 6 bits

#define TRUE 1
#define FALSE 0
#define vpn_mask 0xFC00
#define offset_mask 0x03FF
#define vpn_shift 0xA
int VPN;
int PFN;
int pagetable;
int offset;
int pagetable_size;
int virtual_page_number;
int virtual_address;
int physical_address;
int PTE;
int PTE_address;


// used for page table entries
typedef struct{
	unsigned int valid_bit : 1;
	unsigned int present_bit : 1;
	unsigned int protection_bit : 2;
	unsigned int dirty_bit : 1;
	unsigned int user_bit : 1;
	unsigned int page_frame_number : 14;
} page_table_entry;

typedef struct{
	unsigned int vpn : 6;
	unsigned int offset : 10;	
} virtual_address_space;





void segmentation_fault(page_table_entry *a1){
	//if(a1->valid_bit != 1 ||  a1.present_bit != 1 || a1.protection_bit !=2 || a1.dirty_bit != 1 || a1.user_bit !=1 || a1.page_frame_number !=14 || b1.virtual_frame_numer !=6 || offset !=10){
		
	if(a1->valid_bit != 1 ){
		printf("SEGMENTATION_FAULT\n");
		exit(0);

	}
	else if(a1->protection_bit != 2 ){
		printf("PROTECTION_FAULT\n");
		exit(0);

	}
}

int main(){

 double final_time =0;

 page_table_entry *PTE =  malloc(10*sizeof(page_table_entry));
 virtual_address_space *VAS = malloc(100*sizeof(virtual_address_space));

 for(int i=0;i<100;i++){
 		PTE[i].protection_bit = rand()%4;
 		PTE[i].valid_bit = rand()%2;
 		PTE[i].present_bit = rand()%2;
 		PTE[i].dirty_bit = rand()%2;
 		PTE[i].user_bit = rand()%2;
 		PTE[i].page_frame_number = rand()%16384;


 }

 for(int p=0;p<10;p++){
 		VAS[p].vpn = rand()%64;
 		VAS[p].offset = rand()%1024;
 }


 for(int g=0; g<10; g++){

 		//double final_time =0;


 		unsigned int vpn_x;
 		unsigned int offset_x;

 		clock_t time_it;
 		time_it = clock();


 		vpn_x = VAS[g].vpn;
 		offset_x = VAS[g].offset;

 		page_table_entry pte = PTE[vpn_x];
 		printf("VPN: %d   , OFFSET: %d        \n", vpn_x , offset_x);

 		if(pte.valid_bit == FALSE){
 			printf("SEGMENTATION_FAULT \n ");
 			//printf("  *NO PHYSICAL ADDRESS* \n");
 		}

 		else if (pte.protection_bit == FALSE){
 			printf("PROTECTION_FAULT  \n");

 		}
 		else{

 			physical_address = (pte.page_frame_number << 10) | offset;
 			printf("PAGE FRAME NUMBER: %d         \n", pte.page_frame_number);
 			printf("PHYSICAL ADDRESS : %d         \n", physical_address);
 			printf("   \n");





 		}
 		time_it = clock() - time_it;
 		final_time += ((double)time_it)/CLOCKS_PER_SEC;

 }
  final_time = final_time/20;
  printf("Average time taken: %f\n", final_time);
 
}
/*
1 //Extract the VPN from the virtual address
2 VPN = (VirtualAddress & VPN_MASK) >> SHIFT
3
4 // Form the address of the page-table entry (PTE)
5 PTEAddr = PTBR + (VPN * sizeof(PTE))
6
7 // Fetch the PTE
8 PTE = AccessMemory(PTEAddr)
9
10 // Check if process can access the page
11 if (PTE.Valid == False)
12 RaiseException(SEGMENTATION_FAULT)
13 else if (CanAccess(PTE.ProtectBits) == False)
14 RaiseException(PROTECTION_FAULT)
15 else
16 // Access is OK: form physical address and fetch it
17 offset = VirtualAddress & OFFSET_MASK
18 PhysAddr = (PTE.PFN << PFN_SHIFT) | offset
19 Register = AccessMemory(PhysAddr)
*/
