
#include<stdio.h>
#include<stdint.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define vpn_mask 0xFC00
#define offset_mask 0x03FF
#define vpn_shift 0xA
#define tlb_size 64
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


//used for translation lookaside buffer
typedef struct{
	unsigned int page_frame_number : 14;
	unsigned int protection_bit : 2;
	unsigned int vpn : 6;
	unsigned int use_x :1;
}translation_lookaside_buffer;

void segmentation_fault(page_table_entry *a1){
	//if(a1->valid_bit != 1 ||  a1.present_bit != 1 ||
	// a1.protection_bit !=2 || a1.dirty_bit != 1 || a1.user_bit !=1 || a1.page_frame_number !=14 || b1.virtual_frame_numer !=6 || offset !=10){
		
	if(a1->valid_bit != 1 ){
		printf("SEGMENTATION_FAULT\n");
		exit(0);

	}
	else if(a1->protection_bit != 2 ){
		printf("PROTECTION_FAULT\n");
		exit(0);

	}
}

void tlb_lookup(translation_lookaside_buffer *s, int vpn, int *success, unsigned int *pfn){
	for(int g=0; g<tlb_size; g++){
		if((int)s[g].vpn == vpn){
			s[g].vpn =1;
			*success = 1;
			*pfn = s[g].page_frame_number;
			return;
		}

	}
	*success = 0;
	return;
}

void tlb_miss(translation_lookaside_buffer *tlb, int vpn, unsigned int pfn, int *last){
	srand(time(0));

	for(int i = *last; i<tlb_size; i++){
		if(tlb[i].use_x == FALSE){
			tlb[i].vpn = vpn;
			tlb[i].page_frame_number = pfn;
			tlb[i].use_x = 1;
			*last = i;
			return;
		}
	}

	for(int i =0; i< *last; i++){
		if(tlb[i].use_x == 0){
			tlb[i].vpn = vpn;
			tlb[i].page_frame_number = pfn;
			tlb[i].use_x = i;
			return;
		}
	}

	for(int i=0;i< tlb_size;i++){
		tlb[i].use_x = 0;
	}

	tlb[*last].vpn = vpn;
	tlb[*last].page_frame_number = pfn;
	tlb[*last].use_x = 1;

	

	return;
}

int main(){
//allocating memory ie. malloc
 page_table_entry *PTE =  malloc(100*sizeof(page_table_entry));
 virtual_address_space *VAS = malloc(20*sizeof(virtual_address_space));
 translation_lookaside_buffer *TLB = malloc(100*sizeof(translation_lookaside_buffer));

 int success = 0, last = 0;
 unsigned int pfn;


// estimating values each struct can take
 for(int i=0;i<100;i++){
 		PTE[i].protection_bit = rand()%4;
 		PTE[i].valid_bit = rand()%2;
 		PTE[i].present_bit = rand()%2;
 		PTE[i].dirty_bit = rand()%2;
 		PTE[i].user_bit = rand()%2;
 		PTE[i].page_frame_number = rand()%16384;
 }

 for(int p=0;p<20;p++){
 		VAS[p].vpn = rand()%64;
 		VAS[p].offset = rand()%1024;
 }



for(int m=0;m<100;m++){

	TLB[m].page_frame_number = rand()%16384;
	TLB[m].protection_bit = rand()%4;
	TLB[m].vpn = rand()%64;
	TLB[m].use_x =rand()%2;
}

double final_time =0;

 for(int g=0; g<20; g++){

 	clock_t time_it;
 	time_it = clock();




 		unsigned int vpn_x;
 		unsigned int offset_x;

 		vpn_x = VAS[g].vpn;
 		offset_x = VAS[g].offset;


 		tlb_lookup(TLB, vpn_x, &success, &pfn);
 		int protection_bit = rand()%2;

 		if(success == TRUE){
		 	if(protection_bit == FALSE){
		 		physical_address = (pfn << 10) | offset_x;
		 		sleep(1);
		 		time_it += 1;
		 	}

		 	else
		 		printf("PROTECTION_FAULT\n");
		}else{
			//TLB MISS
			page_table_entry pte = PTE[vpn_x];

			if(pte.valid_bit == FALSE){
				printf("SEGMENTATION_FAULT   \n");
			}else if(pte.protection_bit == FALSE){
				printf("PROTECTION_FAULT   \n");
				}
			else{
				tlb_miss(TLB, vpn_x, pte.page_frame_number, &last);
				g -= 1;
				sleep(2);
		 		time_it += 2;

		 		

			}

	    }
	    printf("VPN: %d\n",vpn_x);
		printf("PFN: %d\n",PTE->page_frame_number);
		printf("\n");


	    time_it = clock() - time_it;
	    final_time += ((double)time_it)/CLOCKS_PER_SEC;
 }
 final_time = final_time/20;
 printf("Average time taken: %f\n", final_time);

}