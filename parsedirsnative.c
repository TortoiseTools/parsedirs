/**
 * Author:  Mustafa Esat SUDADURAN  
 * Created:   25.10.2020
 * 
 * (c) Copyright by TortoiseTools.
 **/

#include <sys/types.h>
#include "dirent.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>

#ifdef _WIN64
#include <shlwapi.h>
#endif


#define TOTAL_MIME_TYPES 199

#define FINDFILES 0
#define FINDINFILES 1

#define TOTAL_REQUEST_PARAM 7

unsigned int filecounter;

FILE *fpw;

char *query_string;
char *query_stringE;
char *reqparam[TOTAL_REQUEST_PARAM]; 
char *reqcommand;

int sendfile(uint8_t *filepath);
int erasefile(uint8_t *filepath);
int findinfile(uint8_t *filepath , uint8_t *what);
int dirpars(char *arg , int how , uint8_t *plus);


uint8_t sys_message[128];


char mime_list[TOTAL_MIME_TYPES][2][96] = {
{"application/SLA" , 	".stl"},
{"image/svg+xml" , 	".svg"},
{"application/STEP" , 	".step"},
{"application/STEP" , 	".stp"},
{"application/acad" , 	".dwg"},
{"application/andrew-inset" , 	".ez"},
{"application/clariscad" , 	".ccad"},
{"application/drafting" , 	".drw"},
{"application/dsptype" , 	".tsp"},
{"application/dxf" , 	".dxf"},
{"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" , 	".xlsx"},
{"application/i-deas" , 	".unv"},
{"application/java-archive" , 	".jar"},
{"application/mac-binhex40" , 	".hqx"},
{"application/mac-compactpro" , 	".cpt"},
{"application/vnd.ms-powerpoint" , 	".pot"},
{"application/vnd.ms-powerpoint" , 	".pps"},
{"application/vnd.ms-powerpoint" , 	".ppt"},
{"application/vnd.ms-powerpoint" , 	".ppz"},
{"application/msword" , 	".doc"},
{"application/octet-stream" ,	".bin"},
{"application/octet-stream" , 	".class"},
{"application/octet-stream" , 	".dms"},
{"application/x-msdownload" , 	".exe"},
{"application/octet-stream" , 	".lha"},
{"application/octet-stream" , 	".lzh"},
{"application/oda" , 	".oda"},
{"application/ogg" , 	".ogg"},
{"application/ogg" , 	".ogm"},
{"application/pdf" , 	".pdf"},
{"application/pgp" , 	".pgp"},
{"application/postscript" , 	".eps"},
{"application/postscript" , 	".ps"},
{"application/pro_eng" , 	".prt"},
{"application/rtf" , 	".rtf"},
{"application/set" , 	".set"},
{"application/smil" , 	".smi"},
{"application/smil" , 	".smil"},
{"application/solids" , 	".sol"},
{"application/vda" , 	".vda"},
{"application/vnd.mif" , 	".mif"},
{"application/vnd.ms-excel" , 	".xlc"},
{"application/vnd.ms-excel" , 	".xll"},
{"application/vnd.ms-excel" , 	".xlm"},
{"application/vnd.ms-excel" , 	".xls"},
{"application/vnd.ms-excel" , 	".xlw"},
{"application/vnd.rim.cod" , 	".cod"},
{"application/x-arj-compressed" , 	".arj"},
{"application/x-bcpio" , 	".bcpio"},
{"application/x-cdlink" , 	".vcd"},
{"application/x-chess-pgn" , 	".pgn"},
{"application/x-cpio" , 	".cpio"},
{"application/x-csh" , 	"csh"},
{"application/x-debian-package" , 	".deb"},
{"application/x-director" , 	".dcr"},
{"application/x-director" , 	".dir"},
{"application/x-director" , 	".dxr"},
{"application/x-dvi" , 	"dvi"},
{"application/x-freelance" , 	".pre"},
{"application/x-futuresplash" , 	".spl"},
{"application/x-gtar" , 	".gtar"},
{"application/x-hdf" , 	"hdf"},
{"application/x-ipix" , 	".ipx"},
{"application/x-ipscript" , 	".ips"},
{"application/x-javascript" , 	".js"},
{"application/x-koan" , 	".skd"},
{"application/x-koan" , 	".skm"},
{"application/x-koan" , 	".skp"},
{"application/x-koan" , 	".skt"},
{"application/x-latex" , 	".latex"},
{"application/x-lisp" , 	".lsp"},
{"application/x-lotusscreencam" , 	".scm"},
{"application/x-mif" , 	".mif"},
{"application/x-msdos-program" , 	".bat"},
{"application/x-msdos-program" , 	".com"},
{"application/x-msdos-program" , 	".exe"},
{"application/x-netcdf" , 	".cdf"},
{"application/x-netcdf" , 	".nc"},
{"application/x-perl" , 	".pl"},
{"application/x-perl" , 	".pm"},
{"application/x-rar-compressed" , 	".rar"},
{"application/x-shar" , 	".shar"},
{"text/plain" , 	".sh"},
{"application/x-shockwave-flash" , 	".swf"},
{"application/x-stuffit" , 	".sit"},
{"application/x-sv4cpio" , 	".sv4cpio"},
{"application/x-sv4crc" , 	".sv4crc"},
{"application/x-tar-gz" , 	".tar.gz"},
{"application/x-gzip" , 	".gz"},
{"application/x-tar-gz" , 	".tgz"},
{"application/x-tar" , 	".tar"},
{"application/x-tcl" , 	".tcl"},
{"text/plain" , 	".tex"},
{"application/x-texinfo" , 	".texi"},
{"application/x-texinfo" , 	".texinfo"},
{"application/x-troff-man" , 	".man"},
{"application/x-troff-ms" , 	".ms"},
{"application/x-troff" , 	".roff"},
{"application/x-troff" , 	".tr"},
{"application/x-ustar" , 	".ustar"},
{"application/x-wais-source" , 	".src"},
{"application/x-zip-compressed" , 	".zip"},
{"application/zip" , 	".zip"},
{"audio/TSP-audio" , 	".tsi"},
{"audio/basic" , 	".au"},
{"audio/basic" , 	".snd"},
{"audio/midi" , 	".kar"},
{"audio/midi" , 	".mid"},
{"audio/midi" , 	".midi"},
{"audio/mpeg" , 	".mp2"},
{"audio/mpeg" , 	".mp3"},
{"audio/mpeg" , 	".mpga"},
{"audio/ulaw" , 	".au"},
{"audio/x-aiff" , 	".aif"},
{"audio/x-aiff" , 	".aifc"},
{"audio/x-aiff" , 	".aiff"},
{"application/postscript" , 	".ai"},
{"audio/x-mpegurl" , 	".m3u"},
{"audio/x-ms-wax" , 	".wax"},
{"audio/x-ms-wma" , 	".wma"},
{"audio/x-pn-realaudio-plugin" , 	".rpm"},
{"audio/x-pn-realaudio" , 	".ram"},
{"audio/x-pn-realaudio" , 	".rm"},
{"audio/x-wav" , 	".wav"},
{"chemical/x-pdb" , 	".pdb"},
{"chemical/x-pdb" , 	".xyz"},
{"image/cmu-raster" , 	".ras"},
{"audio/x-realaudio" , 	".ra"},
{"image/gif" , 	".gif"},
{"image/ief" , 	".ief"},
{"image/jpeg" , 	".jpe"},
{"image/jpeg" , 	".jpeg"},
{"image/jpeg" , 	".jpg"},
{"image/png" , 	".png"},
{"image/tiff" , 	".tif"},
{"image/tiff" , 	".tiff"},
{"image/x-cmu-raster" , 	".ras"},
{"image/x-portable-anymap" , 	".pnm"},
{"image/x-portable-bitmap" , 	".pbm"},
{"image/x-portable-graymap" , 	".pgm"},
{"image/x-portable-pixmap" , 	".ppm"},
{"image/x-rgb" , 	".rgb"},
{"image/x-xbitmap" , 	".xbm"},
{"image/x-xpixmap" , 	".xpm"},
{"image/x-xwindowdump" , 	".xwd"},
{"model/iges" , 	".iges"},
{"model/iges" , 	".igs"},
{"model/mesh" , 	".mesh"},
{"application/x-troff-me" , 	".me"},
{"model/mesh" , 	".msh"},
{"model/mesh" , 	".silo"},
{"model/vrml" , 	".vrml"},
{"model/vrml" , 	".wrl"},
{"text/css" , 	".css"},
{"text/html" , 	".htm"},
{"text/html" , 	".html"},
{"text/plain" , 	".asc"},
{"text/plain" , 	".cc"},
{"text/plain" , 	".c"},
{"text/plain" , 	".f90"},
{"text/plain" , 	".hh"},
{"text/plain" , 	".h"},
{"text/plain" , 	".txt"},
{"text/richtext" , 	".rtx"},
{"text/rtf" , 	".rtf"},
{"text/sgml" , 	".sgm"},
{"text/sgml" , 	".sgml"},
{"text/tab-separated-values" , 	".tsv"},
{"text/vnd.sun.j2me.app-descriptor" , 	".jad"},
{"text/x-setext" , 	".etx"},
{"text/xml" , 	".xml"},
{"video/dl" , 	".dl"},
{"video/fli" , 	".fli"},
{"video/flv" , 	".flv"},
{"video/gl" , 	".gl"},
{"video/mpeg" , 	".mp2"},
{"video/mp4" , 	".mp4"},
{"video/mpeg" , 	".mpe"},
{"video/mpeg" , 	".mpeg"},
{"video/mpeg" , 	".mpg"},
{"video/quicktime" , 	".mov"},
{"video/quicktime" , 	".qt"},
{"video/vnd.vivo" , 	".viv"},
{"video/vnd.vivo" , 	".vivo"},
{"video/x-fli" , 	".fli"},
{"text/plain" , 	".f"},
{"video/x-ms-asf" , 	".asf"},
{"video/x-ms-asx" , 	".asx"},
{"video/x-ms-wmv" , 	".wmv"},
{"video/x-ms-wmx" , 	".wmx"},
{"video/x-ms-wvx" , 	".wvx"},
{"video/x-msvideo" , 	".avi"},
{"video/x-sgi-movie" , 	".movie"},
{"www/mime" 	,  ".mime" },
{"text/plain" , 	".m"},
{"x-conference/x-cooltalk" , 	".ice"},
{"x-world/x-vrml" , 	".vrm"},
{"x-world/x-vrml" , 	".vrml"},
{"application/x-troff" , 	".t"},
};




int percent_decode(char* out, const char* in) 
 
{ 
static const char tbl[256] = 
{ -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1, -1,10,11,12,13,14,15,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,10,11,12,13,14,15,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1 }; 

	char c, v1, v2, *beg=out; 

	if(in != NULL) { 
		while((c=*in++) != '\0') 
		{ 
			if(c == '%') { 
				if(!(v1=*in++) || (v1=tbl[(unsigned char)v1])<0 || !(v2=*in++) || (v2=tbl[(unsigned char)v2])<0) 
				{ 
					*beg = '\0'; return -1; 
				} 
				c = (v1<<4)|v2; 
			}
			 *out++ = c; 
if(*(out-1) == '+')
{
*(out-1) = ' ';
}
		} 
	} 
	*out = '\0'; 
	return 0;
}
/**
 * Web Client tarafindan cgi-bin metoduyla gonderilen istegi; komut
 * ve parametrelerine pars eder. Komut **reqcommand** external pointer degiskenine, 
 * parametreler **reqparam** pointer dizisine atanir.
 * @return	Bulunan parametre sayisi doner.
 * @note	Fonksiyon parametreleri eklenerek bu fonksiyonun optimize 
 * edilmesi planlanmakta.
 * @todo   Bu fonksiyona hata donusleri eklenmeli.
 *		
 */
static int parserequest()
{  
	int i, parsay=0;
	reqcommand = query_string;

	if((reqparam[0] = strchr(reqcommand, '&')) != NULL)
	{ 
		*reqparam[0] = '\0';
		reqparam[0] = reqparam[0] + 1;
		parsay++;
	}
	else{
		return parsay;
	}

	for(i=1 ; i<TOTAL_REQUEST_PARAM ; i++){
		if((reqparam[i] = strchr(reqparam[i-1], '&')) != NULL)
		{
			*reqparam[i] = '\0';
			reqparam[i] = reqparam[i] + 1;
			parsay++;
		}
		else{
			return parsay;
		}
	}

	return parsay;
}





int erasefile(uint8_t *filepath)
{
	FILE *fpr;
	uint64_t filesize;
        uint8_t *findpointer;
	uint8_t c;
	int i;

	findpointer = strchr(filepath , '\n');

	if(findpointer)
	{
		*findpointer = 0;
	}

	i = remove(filepath);

	if(i == 0)
	{
		//printf("Content-Type:text/plain\n\n");	
		printf("Successfull : %s file erased !" , filepath);
	}
	else{
		//printf("Content-Type:text/plain\n\n");	
		printf("Error remove %s : %s\n" ,filepath, strerror(errno));
	}

	return 0;

}





int sendfile(uint8_t *filepath)
{
	FILE *fpr;
	uint64_t filesize;
        uint8_t *findpointer;
	uint8_t c;
	int i;

	findpointer = strchr(filepath , '\n');

	if(findpointer)
	{
		*findpointer = 0;
	}

//memset(sys_message, '\0', 128);
//sprintf(sys_message , "chmod 555 %s", filepath);
//system(sys_message);

 	fpr = fopen(filepath,"rb");

	if(fpr == NULL)
	{
		//printf("Content-Type:text/plain\n\n");	
		printf("Error openfile %s : %s\n" ,filepath, strerror(errno));
		return 0;
	}

	c=0;

	for(i = 0 ; i < TOTAL_MIME_TYPES ; i++)
	{
		if(strstr(filepath , mime_list[i][1]))
		{
			printf("Content-Type:%s\n\n" , mime_list[i][0]);
			c = 7;
			break;
		}
	}

        if(c!=7)
	{
		printf("Content-Type:text/plain\n\n");	
	}	
	fseek(fpr,0,SEEK_END);
	filesize = ftell(fpr);
	fseek(fpr,0,SEEK_SET);

	freopen(NULL, "wb", stdout);
if(filesize == 0)
{
	filesize = 4096;
}
	while(filesize)
	{
		c=getc(fpr);
	 	putc(c,stdout);
		filesize--;
	}

freopen(NULL, "w", stdout);
fclose(fpr); 
return 0;

}













int findinfile(uint8_t *filepath , uint8_t *what)
{
	FILE *fpr;
	uint64_t filesize;
    uint8_t *filepointer;
	uint8_t *findpointer;
	uint8_t *findlinestartpointer;
	uint8_t *findlineendpointer;
	uint8_t *findlinenumber;
	uint32_t linenumber;
	uint8_t findatleastoneflag = 0;

	findpointer = strchr(filepath , '\n');

	if(findpointer)
	{
		*findpointer = 0;
	}

 	fpr = fopen(filepath,"rb");

	if(fpr == NULL)
	{	
		printf("Error openfile %s : %s\n" ,filepath, strerror(errno));
		return 0;
	}

	fseek(fpr,0,SEEK_END);
	filesize = ftell(fpr);
	fseek(fpr,0,SEEK_SET);
	if(filesize == 0)
	{
		fclose(fpr); 
		filesize = 4096;
		return 0;
	}
	filepointer = malloc(filesize+1);
	if(filepointer == NULL){
		printf("Error : Too large data... %s\n" ,filepath);
		fclose(fpr); 
		return 0;
	}

	memset(filepointer, '\0', filesize+1);
   	fread(filepointer, sizeof(uint8_t), filesize, fpr);
   	
    findpointer = filepointer;
    
    while(findpointer)
    {

#ifdef _WIN64
	findpointer = StrStrIA(findpointer , what);
#endif 
	    
#ifdef __linux__
		findpointer = strcasestr(findpointer , what);
#endif 
	
		if(findpointer != NULL)
		{
			if(!findatleastoneflag)	
			{	
				findatleastoneflag = 1;
				printf("%s ---- %s dosyasinda bulundu.\n" , what ,filepath);
			}		
			
			findlinestartpointer = findpointer;
			findlineendpointer = findpointer;
			
			/***satir baslangicini bul***/
			while(findlinestartpointer >= filepointer)
			{
				if((*findlinestartpointer) == '\n') 
				{
						break;
				}	
				findlinestartpointer--;
			}	
			
			
				findlinestartpointer++;
		
			/*****************************/
			
			
			
			/***satir sonunu bul***/
			while(findlineendpointer <= filepointer+filesize)
			{
				if((*findlineendpointer) == '\n') 
				{
						break;
				}	
				findlineendpointer++;
			}	
			
	
				findlineendpointer--;
		
			/*****************************/
			
			/***satir numarasi bul***/
			linenumber = 1;
			for(findlinenumber = filepointer ; findlinenumber < findpointer ;findlinenumber++)
			{
				if((*findlinenumber) == '\n')
				{
					linenumber++;
					
				}
			}		
			/************************/

			printf("\tLine %d : %.*s\n",   linenumber, findlineendpointer - findlinestartpointer ,findlinestartpointer);
			findpointer = findlineendpointer;
		}
	}
	
free(filepointer);
fclose(fpr); 
return 0;

}






















int dirpars(char *arg , int how , uint8_t *plus)
{
	int fc;
	DIR *dirc;
	struct dirent *direntc;
        char Path[1024];
	char FilePath[1024];
	char buf[128];

	/***On Hazirlik***/
	if(arg == NULL)
	{
	  printf("Error : Dosya ismi/yolu girilmedi\n");
          return 0;
	}
	sprintf(Path , "%s", arg);
	/****************/

	/***Klasor olustur***
	fc = mkdir( Path , 0777);
	if(fc == -1)
	{
		printf("Warning mkdir  : %s\n" , strerror(errno));
	}
	****************/

	/***Klasoru ac***/
	dirc = opendir(Path);

	if(dirc == NULL)
	{
		/*if(errno == EACCES)
		{
			sprintf(buf, "sudo chmod -R 777 %s", Path);

			system(buf);

			dirc = opendir(Path);
		}*/

		if(dirc == NULL)
		{
			printf("Error opendir %s : %s\n" ,Path, strerror(errno));
			return 0;
		}
	}
	/****************/

	/***Klasoru oku***/
	if(how == FINDFILES){
		printf("DIRECTORY : %s\n" , Path);
	}

	while(direntc = readdir(dirc))
	{
                /***Bulunan dosyanin ismini ve ozelliklerini yazdir***/
		if(how == FINDFILES && !strcmp(plus, "detay")){
			printf("File name : %s/%s\nFile type : %d\nFile record length : %d\nNextFile offset : %lu\nFile inode number : %lu \n\n\n" , arg,direntc->d_name , direntc->d_type , 			direntc->d_reclen , direntc->d_off , direntc->d_ino);
		}
		/****************************************************/

 		/***Bulunan dosya klasor cinsinden degilse bulunan dosya sayacini artir ve dosya ismini 'dirprocessresult' dosyasina yaz***/
		if(direntc->d_type != DT_DIR)
		{
			if(how == FINDFILES){
				if(!strcmp(plus, "detay"))
				{
					printf("%s/%s : type=%d\n", arg , direntc->d_name, direntc->d_type);
				}
				else
				{
					printf("%s/%s\n", arg , direntc->d_name);
				}
			}

	
			if(how == FINDINFILES && direntc->d_type == DT_REG){
				sprintf(FilePath , "%s/%s\n", arg , direntc->d_name);			
				findinfile(FilePath , plus);
			}
			//fpw = fopen("dirprocessresult","a+");
			//sprintf(FilePath , "%s/%s\n", arg , direntc->d_name);
			//fwrite(FilePath , 1, strlen(FilePath), fpw);
			//fflush(fpw); //onbellek temizlenir
			//fclose(fpw); //Dosya kapatilir
			filecounter++;
		}
		/****************************************************************************************************************************/

		/***Bulunan dosya klasor cinsinden ise bu klasorude pars et***/
	        if(direntc->d_type == DT_DIR  && direntc->d_name[0] != '.')
		{
			sprintf(Path , "%s/%s", arg , direntc->d_name);
 			dirpars(Path , how , plus);
		} 
		/*************************************************************/
	}
	/****************/

	/***Klasoru kapat***/
	closedir(dirc);
	/*******************/

	return 0;
}




char terminal[1024];

int main(int argc , char *argv[])
{
	int fc;
	DIR *dirc;
	struct dirent *direntc;
    char Path[1024];
	char SearchingSentence[256];
	char *fileparam1;
	char *fileparam2;
	int t;

	/**********parse request************/
	memset(terminal , 0 , 1024);
	
	sprintf(terminal, "%s" , argv[1]);	
	for(t=2 ; t<15 ; t++ )
	{
		if(argv[t])
		{
			sprintf(&terminal[strlen(terminal)] , "&%s" , argv[t]);	
		}
		else
		{
			break;
		}
		
		query_stringE = terminal;
	}
if(query_stringE == NULL){
	//printf("Content-Type:text/plain\n\n");
	printf("Error : query_string fail!\n\n");
	return 0;
}
query_string = malloc(256);
fc = percent_decode(query_string ,query_stringE);
if(fc == -1){
	//printf("Content-Type:text/plain\n\n");
	printf("Error : url decode fail!\n\n");
	return 0;
}

/*	query_string = getenv("QUERY_STRING");*/

	if(query_string == NULL){
		//printf("Content-Type:text/plain\n\n");
		printf("Error : query_string fail!\n\n");
		return 0;
	}
	else{
		parserequest();
	}
	/********************************/

	if (!strncmp(query_string, "findfiles" , 9)) {
		//printf("Content-Type:text/plain\n\n");
		if(reqparam[0] == NULL)
		{
		  printf("Error : Dosya ismi/yolu girilmedi\n");
		  return 0;
		}
		sprintf(Path , "%s", reqparam[0]);

		filecounter = 0;
		if(reqparam[1] == NULL){
			dirpars(Path , FINDFILES , "0");
		}
		else{
			dirpars(Path , FINDFILES , reqparam[1]);
		}
		/***read-only file system hatasina karsi yapildi***/
	//	system("mount -o rw,remount /dev/mmcblk0p2");
		/**************************************************/
		printf("Dosya Sayisi : %d" , filecounter);
	}
	else if (!strncmp(query_string, "findinfiles" , 11)) {
		//printf("Content-Type:text/plain\n\n");
		if(reqparam[0] == NULL)
		{
		  printf("Error : Dosya ismi/yolu girilmedi\n");
		  return 0;
		}

		if(reqparam[1] == NULL)
		{
		  printf("Error : Aranacak kelime girilmedi\n");
		  return 0;
		}
		sprintf(Path , "%s", reqparam[0]);
		memset(SearchingSentence, '\0', 256); 
		sprintf(SearchingSentence , "%s", reqparam[1]);

		filecounter = 0;
		dirpars(Path , FINDINFILES , SearchingSentence);


		/***read-only file system hatasina karsi yapildi***/
	//	system("mount -o rw,remount /dev/mmcblk0p2");
		/**************************************************/
		printf("Dosya Sayisi : %d" , filecounter);
	}
	else if (!strncmp(query_string, "sendfile" , 8)) {
		if(reqparam[0] == NULL)
		{
		//printf("Content-Type:text/plain\n\n");
		  printf("Error : Dosya ismi/yolu girilmedi\n");
		  return 0;
		}

		if(strcmp(reqparam[0] , "0x33") == 0)
		{
			memset(SearchingSentence, '\0', 256); 
			fread(SearchingSentence, sizeof(uint8_t), 256, stdin);
			fileparam1 = strstr(SearchingSentence,"sendfile");
			fileparam2 = strchr(fileparam1 + 13, 0x0D);
			if(fileparam2)
			{
				*fileparam2 = '\0';
			}
			sprintf(Path , "%s", fileparam1 + 13);
		}
		else
		{
			if(reqparam[0][0] == '=')
			{
				sprintf(Path , "%s", reqparam[0]+1);
			}
			else
			{
				sprintf(Path , "%s", reqparam[0]);
			}
		}

		sendfile(Path);
	}
	else if (!strncmp(query_string, "erasefile" , 9)) {
		if(reqparam[0] == NULL)
		{
		//printf("Content-Type:text/plain\n\n");
		  printf("Error : Dosya ismi/yolu girilmedi\n");
		  return 0;
		}

		sprintf(Path , "%s", reqparam[0]);

		erasefile(Path);
	}
	else  {
			//printf("Content-Type:text/plain\n\n");
			printf("Error : Gecersiz istek\n");

			return 0;
		
	}
free(query_string);
	return 0;

}





