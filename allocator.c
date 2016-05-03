//Bucur Ionut-Cristian,313CA

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char *Arena;//initializam arena global ca sa putem s-o folosim si prin functii fara probleme stresante

void INITIALIZE(int DimensiuneArena){ // alocarea dinamica a memoriei pentru arena de dimensiune N
	Arena=calloc(DimensiuneArena,sizeof(unsigned char));
	unsigned char *IndexStart;
	IndexStart=Arena;
	*IndexStart=0;// initializare index de start ca int care se comporta in acelasi fel ca unsigned char in acest caz

}

void FINALIZE(){
	free(Arena);//eliberam zona de memorie alocata arenei
}

void DUMP(int DimensiuneArena){
	int n,i,j=0,stop;
	
		n=DimensiuneArena/16;//scoatem numaru' de linii pe care o sa se intinda afisarea octetilor din arena

	for(i=0;i<n;i++){
		printf("%08X",(16*i));//afisam indicele curent in formatul cerut
		printf("\t");
		stop=16;
		while(stop){//printf("j:%d ",j);
			printf("%02X ",*(Arena+j));//afisam octetii din arena
			if(stop==9){
				printf(" ");//afisam un spatiu suplimentar dupa 
			};
			stop--;
			j++;	
		}
		printf("\n");
	}
	printf("%08X",(16*i));//afisam numerotarea din margine
	printf("\t");
	for(i=0;i<(DimensiuneArena%16);i++){
		printf("%02X ",*(Arena+j));//afisam restu de octetii din arena -ultima linie in caz ca nu e plina(16 octeti)
		j++;
	}
	printf("\n");
	printf("%08X",DimensiuneArena);//afisam dimensiunea arenei pe ultimul rand
	printf("\n");
}

void FILL(int index,int size,int value){//bagam value in size octeti din arena incepand cu index
	int i;
	for(i=0;i<size;i++){
		*(Arena+index+i)=value;//bagam value in elementul index+i al Arenei
	}
}

int ALLOC(int size,int DimensiuneArena){
	int IndexCurent,IndexUrmator,SpatiuIntrePozitiActuale,IndexStart,SizeCurent;
	IndexStart=*Arena;
	
	if(DimensiuneArena<size+12){//daca marimea arenei este prea mica sau marimea blocului de introdus e prea mare,nu are rost sa continuam
		return 0;
	}
	if(IndexStart==0){//in caz ca arena e goala

		*Arena=4;//bagam adresa noului bloc in indexul de start
		*(Arena+4)=0;//bagam adresa indexului de start,specifica primului bloc
		*(Arena+8)=0;//bagam adresa urmatorului bloc,dar fiind ultimul e 0
		*(Arena+12)=size+12;//bagam marimea(size+gestiune)
		return 16;
	}

	if((IndexStart-4)>=(size+12)){//intre index start si prima
		*(Arena+4)=*Arena;
		*(Arena+8)=0;
		*(Arena+12)=size+12;
		*(Arena+(*(Arena+4)+4))=4;
		*Arena=4;
		return 16;
	}
	IndexCurent=*(Arena);
	IndexUrmator=*(Arena+IndexCurent);
	SizeCurent=*(Arena+IndexCurent+8);
	while(1){//parcurgeam toata Arena pentru a gasi un loc liber
		if(IndexUrmator==0){//am ajuns la ultimu' bloc
			if(DimensiuneArena-IndexCurent-SizeCurent>=size+12){//facem modificarile aferente ultimului bloc in caz ca e loc
				*(Arena+IndexCurent+SizeCurent)=IndexUrmator;
				*(Arena+IndexCurent+SizeCurent+4)=IndexCurent;
				*(Arena+IndexCurent+SizeCurent+8)=size+12;
				*(Arena+IndexCurent)=IndexCurent+SizeCurent;
				return (IndexCurent+SizeCurent+12);
		
			}
		return 0;//returnam 0 daca n-am gasit loc
		}


		
		if((IndexUrmator-IndexCurent-SizeCurent)>=(size+12)){//intre 2 blocuri
			*(Arena+IndexCurent+SizeCurent)=IndexUrmator;
			*(Arena+IndexCurent+SizeCurent+4)=IndexCurent;
			*(Arena+IndexCurent+SizeCurent+8)=size+12;
			*(Arena+IndexCurent)=IndexCurent+SizeCurent;
			*(Arena+IndexUrmator+4)=IndexCurent+SizeCurent;
			return (IndexCurent+SizeCurent+12);
		}

		IndexCurent=IndexUrmator;//modificari pentru a putea trece la urmatorul index
		IndexUrmator=*(Arena+IndexCurent);
		SizeCurent=*(Arena+IndexCurent+8);		
	}

	return 0;
}


int FREE(int index,int DimensiuneArena){
	if(index<=DimensiuneArena){
		*(Arena+(*(Arena+index-8)))=*(Arena+index-12);//eliberam blocul respectiv,marcand acest lucru in blocul
		*(Arena+(*(Arena+index-12)+4))=*(Arena+index-8);//precedent si urmator,sarind astfel peste acesta

	
	if(*(Arena+index-8)==0){//cazul in care blocul este primul din arena
		*Arena=*(Arena+index-12);

		return 0;
	}return 0;
}
}

int SHOWFREE(int DimensiuneArena){
	int NumarBlocuri=0,SpatiulLiber=0,IndexStart=*Arena,IndexUrmator,IndexCurent,SizeCurent;
	if(IndexStart==0){//in caz ca arena e goala

	printf("1 blocks (%d bytes) free\n",DimensiuneArena);
	return 0;
	}
	
	IndexStart=*Arena;
	if((IndexStart-4)>0){//intre index start si prima
	NumarBlocuri++;//numaram spatiile goale continue
	SpatiulLiber=SpatiulLiber+(IndexStart-4);//adaugam spatiul liber la cel total 
	}
	IndexCurent=*(Arena);
	IndexUrmator=*(Arena+IndexCurent);
	SizeCurent=*(Arena+IndexCurent+8);
	while(1){//parcurgem arena
		if(IndexUrmator==0){
			if(DimensiuneArena-IndexCurent-SizeCurent>0){//verificam spatiul intre ultimul bloc si sfarstiul arenei
				NumarBlocuri++;
				SpatiulLiber=SpatiulLiber+(DimensiuneArena-IndexCurent-SizeCurent);//calculam acest spatiu
				printf("%d blocks (%d bytes) free\n",NumarBlocuri,SpatiulLiber);//afisam spatiu aflat
				
				break;//dupa care iesim din loop
		
			}
		}
		
		if((IndexUrmator-IndexCurent-SizeCurent)>0){//verificam spatiul intre 2 blocuri daca exista
			NumarBlocuri++;
			SpatiulLiber=SpatiulLiber+(IndexUrmator-IndexCurent-SizeCurent);//il adaugam la total

		}

		IndexCurent=IndexUrmator;
		IndexUrmator=*(Arena+IndexCurent);
		SizeCurent=*(Arena+IndexCurent+8);		
	}

	return 0;
}
int SHOWUSAGE(int DimensiuneArena){
	int NumarBlocuri=0,SpatiulFolosit=0,IndexStart=*Arena,IndexUrmator,IndexCurent,SizeCurent,NumarBlocuriLibere=0;
	float eficienta,OctetiRezervati,fragmentare;
	if(IndexStart==0){//in caz ca arena e goala

	printf("0 blocks (0 bytes) used\n");
	printf("100%% efficiency\n");
	printf("0%% fragmentation\n");
	return 0;
	}
	IndexStart=*Arena;
	if((IndexStart-4)>0){//intre index start si prima
	NumarBlocuriLibere++;//numaram blocurile libere
	}
	IndexCurent=*(Arena);
	IndexUrmator=*(Arena+IndexCurent);
	SizeCurent=*(Arena+IndexCurent+8);
	while(1){//parcurgem arena pentru a numara blocurile libere
		if(IndexUrmator==0){
			if(DimensiuneArena-IndexCurent-SizeCurent>0){//intre ultimu' si capat
				NumarBlocuriLibere++;

				
				break;
		
			}
		}
		
		if((IndexUrmator-IndexCurent-SizeCurent)>0){//intre 2 indexi
			NumarBlocuriLibere++;


		}

		IndexCurent=IndexUrmator;
		IndexUrmator=*(Arena+IndexCurent);
		SizeCurent=*(Arena+IndexCurent+8);		
	}
	
	IndexCurent=*(Arena);
	IndexUrmator=*(Arena+IndexCurent);
	SizeCurent=*(Arena+IndexCurent+8);
	while(1){//parcurgem arena pentru a numara blocurile ocupate in (NumarBlocuri) si spatiul folosit de acestea
		if(IndexUrmator==0){//daca ajungem la final
				NumarBlocuri++;// numaram si acest bloc 
				SpatiulFolosit=SpatiulFolosit+SizeCurent-12;//si spatiul folosit de acesta
				printf("%d blocks (%d bytes) used\n",NumarBlocuri,SpatiulFolosit);//afisam spatiul folosit
				OctetiRezervati=NumarBlocuri*12+4+SpatiulFolosit;//calculam octetii rezervati
				eficienta=(float)((SpatiulFolosit/OctetiRezervati)*100);//calculam eficienta
				printf("%d%% efficiency\n",(int)eficienta);//o afisam
				fragmentare=((NumarBlocuriLibere-1)/NumarBlocuri)*100;//calculam fragmentarea
				printf("%d%% fragmentation\n",(int)fragmentare);//o afisam
				break;//iesim din loop
		
			}

			NumarBlocuri++;//numaram blocurile ocupate
			SpatiulFolosit=SpatiulFolosit+SizeCurent-12;//calculam spatiul total folosit



		IndexCurent=IndexUrmator;
		IndexUrmator=*(Arena+IndexCurent);
		SizeCurent=*(Arena+IndexCurent+8);		
	}

	return 0;
}
int SHOWALLOCATIONS(int DimensiuneArena){
	int SpatiulLiber=0,IndexStart=*Arena,IndexUrmator,IndexCurent,SizeCurent,SpatiulFolosit=0;

	if(IndexStart==0){//in caz ca arena e goala
	printf("FREE %d bytes\n",DimensiuneArena);
	return 0;
	}
	printf("OCCUPIED 4 bytes\n");
	IndexStart=*Arena;
	if((IndexStart-4)>0){//verificam daca este un spatiu liber intre index start si prima
	SpatiulLiber=IndexStart-4;
	printf("FREE %d bytes\n",SpatiulLiber);
	}

	IndexCurent=*(Arena);
	IndexUrmator=*(Arena+IndexCurent);
	SizeCurent=*(Arena+IndexCurent+8);
	while(1){//parcurgeam arena
		if(IndexUrmator==0){//ultimul bloc
			if(DimensiuneArena-IndexCurent-SizeCurent>0){//verificam daca spatiul este ocupat si il afisam
				SpatiulLiber=DimensiuneArena-IndexCurent-SizeCurent;
				printf("OCCUPIED %d bytes\n",SpatiulLiber);
		
			}

				SpatiulFolosit=DimensiuneArena-IndexCurent-SizeCurent;//dupa ce il calculam
				printf("FREE %d bytes\n",SpatiulFolosit);//afisam spatiul liber de la final
				break;
		

		}
		
		if((IndexUrmator-IndexCurent-SizeCurent)>0){//verificam daca exista spatiu liber intre 2 blocuri
			SpatiulLiber=IndexUrmator-IndexCurent-SizeCurent;
			printf("FREE %d bytes\n",SpatiulLiber);
		}
		if((IndexUrmator-IndexCurent-SizeCurent)==0){//sau daca este tot ocupat
		

			SpatiulFolosit=SizeCurent;
			printf("OCCUPIED %d bytes\n",SpatiulFolosit);
		}


		
		

		IndexCurent=IndexUrmator;
		IndexUrmator=*(Arena+IndexCurent);
		SizeCurent=*(Arena+IndexCurent+8);		
	}

	return 0;
}

int main(){
	while(1){//meniu sexos
		int DimensiuneArena;
		char Comanda[100];
		scanf("%s",Comanda);//citim comenzile si apelam functiile creeate anterior spre rezolvarea acestora
		if(strcmp(Comanda,"INITIALIZE")==0){
			int N;
			scanf("%d",&N);
			DimensiuneArena=N;
			INITIALIZE(DimensiuneArena);
		}
		if(strcmp(Comanda,"FINALIZE")==0){
			FINALIZE();
			break;
		}
		if(strcmp(Comanda,"DUMP")==0){
			DUMP(DimensiuneArena);
		}
		if(strcmp(Comanda,"SHOW")==0){
			char *info=malloc(20*sizeof(char));
			scanf("%s",info);
			if(strcmp(info,"FREE")==0){

			SHOWFREE(DimensiuneArena);
			}
			if(strcmp(info,"USAGE")==0){
	
			SHOWUSAGE(DimensiuneArena);
			}
			if(strcmp(info,"ALLOCATIONS")==0){
			SHOWALLOCATIONS(DimensiuneArena);
			}
			free(info);
		}
		if(strcmp(Comanda,"FREE")==0){
			int index;
			scanf("%d",&index);
			FREE(index,DimensiuneArena);
		}
		if(strcmp(Comanda,"FILL")==0){
			int Index,Size,Value;
			scanf("%d",&Index);
			scanf("%d",&Size);
			scanf("%d",&Value);
			FILL(Index,Size,Value);
		}
		if(strcmp(Comanda,"ALLOC")==0){
			int size;
			scanf("%d",&size);
			printf("%d",ALLOC(size,DimensiuneArena));
			printf("\n");
		}
	}
}
