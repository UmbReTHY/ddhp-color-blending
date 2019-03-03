#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "modelReader.hpp"
#include "svm.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
svm_model* ModelReader::fromString(const char modelText[]) const {
  // read parameters
  svm_model *model = Malloc(svm_model,1);
  svm_parameter& param = model->param;
  model->rho = NULL;
  model->probA = NULL;
  model->probB = NULL;
  model->label = NULL;
  model->nSV = NULL;

  std::stringstream modelFileStream(modelText);
  char cmd[81];
  while(1)
  {
    //fscanf(fp,"%80s",cmd);
    modelFileStream >> std::skipws >> std::setw(80) >> cmd;

    if(strcmp(cmd,"svm_type")==0)
    {
	    //fscanf(fp,"%80s",cmd);
      modelFileStream >> std::skipws >> std::setw(80) >> cmd;
	    int i;
	    for(i=0;svm_type_table[i];i++)
	    {
		    if(strcmp(svm_type_table[i],cmd)==0)
		    {
			    param.svm_type=i;
			    break;
		    }
	    }
	    if(svm_type_table[i] == NULL)
	    {
		    fprintf(stderr,"unknown svm type.\n");
		    free(model->rho);
		    free(model->label);
		    free(model->nSV);
		    free(model);
		    return NULL;
	    }
    }
    else if(strcmp(cmd,"kernel_type")==0)
    {		
	    //fscanf(fp,"%80s",cmd);
      modelFileStream >> std::skipws >> std::setw(80) >> cmd;
	    int i;
	    for(i=0;kernel_type_table[i];i++)
	    {
		    if(strcmp(kernel_type_table[i],cmd)==0)
		    {
			    param.kernel_type=i;
			    break;
		    }
	    }
	    if(kernel_type_table[i] == NULL)
	    {
		    fprintf(stderr,"unknown kernel function.\n");
		    free(model->rho);
		    free(model->label);
		    free(model->nSV);
		    free(model);
		    return NULL;
	    }
    }
    else if(strcmp(cmd,"degree")==0)
	    //fscanf(fp,"%d",&param.degree);
      modelFileStream >> std::skipws >> param.degree;
    else if(strcmp(cmd,"gamma")==0)
	    //fscanf(fp,"%lf",&param.gamma);
      modelFileStream >> std::skipws >> param.gamma;
    else if(strcmp(cmd,"coef0")==0)
	    //fscanf(fp,"%lf",&param.coef0);
      modelFileStream >> std::skipws >> param.coef0;
    else if(strcmp(cmd,"nr_class")==0)
	    //fscanf(fp,"%d",&model->nr_class);
      modelFileStream >> std::skipws >> model->nr_class;
    else if(strcmp(cmd,"total_sv")==0)
	    //fscanf(fp,"%d",&model->l);
      modelFileStream >> std::skipws >> model->l;
    else if(strcmp(cmd,"rho")==0)
    {
	    int n = model->nr_class * (model->nr_class-1)/2;
	    model->rho = Malloc(double,n);
	    for(int i=0;i<n;i++)
		    //fscanf(fp,"%lf",&model->rho[i]);
        modelFileStream >> std::skipws >> model->rho[i];
    }
    else if(strcmp(cmd,"label")==0)
    {
	    int n = model->nr_class;
	    model->label = Malloc(int,n);
	    for(int i=0;i<n;i++)
		    //fscanf(fp,"%d",&model->label[i]);
        modelFileStream >> std::skipws >> model->label[i];
    }
    else if(strcmp(cmd,"probA")==0)
    {
	    int n = model->nr_class * (model->nr_class-1)/2;
	    model->probA = Malloc(double,n);
	    for(int i=0;i<n;i++)
		    //fscanf(fp,"%lf",&model->probA[i]);
        modelFileStream >> std::skipws >> model->probA[i];
    }
    else if(strcmp(cmd,"probB")==0)
    {
	    int n = model->nr_class * (model->nr_class-1)/2;
	    model->probB = Malloc(double,n);
	    for(int i=0;i<n;i++)
		    //fscanf(fp,"%lf",&model->probB[i]);
        modelFileStream >> std::skipws >> model->probB[i];
    }
    else if(strcmp(cmd,"nr_sv")==0)
    {
	    int n = model->nr_class;
	    model->nSV = Malloc(int,n);
	    for(int i=0;i<n;i++)
		    //fscanf(fp,"%d",&model->nSV[i]);
        modelFileStream >> std::skipws >> model->nSV[i];
    }
    else if(strcmp(cmd,"SV")==0)
    {
	    while(1)
	    {
		    //int c = getc(fp);
        int c = modelFileStream.get();
		    if(/*c==EOF*/modelFileStream.eof() || c=='\n') break;	
	    }
	    break;
    }
    else
    {
	    fprintf(stderr,"unknown text in model file: [%s]\n",cmd);
	    free(model->rho);
	    free(model->label);
	    free(model->nSV);
	    free(model);
	    return NULL;
    }
  }

  // read sv_coef and SV

  int elements = 0;
  //long pos = ftell(fp);

  int max_line_len = 1024;
  char line[1024];// = Malloc(char,max_line_len);
  char *p,*endptr,*idx,*val;

  /*while(!modelFileStream.eof()) {
    modelFileStream.getline(line, max_line_len);
    if (0 == modelFileStream.gcount())
      continue;
    p = strtok(line,":");
    while(1)
    {
	    p = strtok(NULL,":");
	    if(p == NULL)
		    break;
	    ++elements;
    }
  }
  elements += model->l;*/
  elements = 8 * model->l;

  //fseek(fp,pos,SEEK_SET);

  int m = model->nr_class - 1;
  int l = model->l;
  model->sv_coef = Malloc(double *,m);
  int i;
  for(i=0;i<m;i++)
    model->sv_coef[i] = Malloc(double,l);
  model->SV = Malloc(svm_node*,l);
  svm_node *x_space = NULL;
  if(l>0) x_space = Malloc(svm_node,elements);
  int j=0;
  for(i=0;i<l;i++)
  {
    //readline(fp);
    modelFileStream.getline(line, max_line_len);
    model->SV[i] = &x_space[j];

    p = strtok(line, " \t");
    model->sv_coef[0][i] = strtod(p,&endptr);
    for(int k=1;k<m;k++)
    {
	    p = strtok(NULL, " \t");
	    model->sv_coef[k][i] = strtod(p,&endptr);
    }

    while(1)
    {
	    idx = strtok(NULL, ":");
	    val = strtok(NULL, " \t");

	    if(val == NULL)
		    break;
	    x_space[j].index = (int) strtol(idx,&endptr,10);
	    x_space[j].value = strtod(val,&endptr);

	    ++j;
    }
    x_space[j++].index = -1;
  }
  //free(line);

  /*if (ferror(fp) != 0 || fclose(fp) != 0)
    return NULL;*/

  model->free_sv = 1;	// XXX
  return model;
}

const char* ModelReader::svm_type_table[6] = {"c_svc","nu_svc","one_class",
                                              "epsilon_svr","nu_svr",NULL};
const char* ModelReader::kernel_type_table[6] = {"linear","polynomial","rbf",
                                                 "sigmoid","precomputed",NULL};

