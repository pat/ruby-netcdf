#include<stdio.h>
#include "ruby.h"
#include "narray.h"
#include<netcdf.h>
#include<string.h>

/* for compatibility with ruby 1.6 */
#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif
#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif
#ifndef RARRAY_PTR
#define RARRAY_PTR(a) (RARRAY(a)->ptr)
#endif
#ifndef RARRAY_LEN
#define RARRAY_LEN(a) (RARRAY(a)->len)
#endif
#ifndef StringValueCStr
#define StringValueCStr(s) STR2CSTR(s)
#endif
#ifndef SafeStringValue
#define SafeStringValue(s) Check_SafeStr(s)
#endif

/* for compatibility for NArray and NArray with big memory patch */
#ifndef NARRAY_BIGMEM
typedef int    na_shape_t;
#endif

/* Data to NArray */

/*    memcpy(ary->ptr,nc_ptr,na_sizeof[NA_SINT]*ary->total); \ */

#define Cbyte_to_NArray(v, rank, shape, up) \
{ \
    struct NARRAY *ary; \
    v = na_make_object(NA_BYTE, rank, shape, cNArray); \
    GetNArray(v,ary); \
    up = (unsigned char *)ary->ptr; \
}

#define Csint_to_NArray(v, rank, shape, sp) \
{ \
    struct NARRAY *ary; \
    v = na_make_object(NA_SINT, rank, shape, cNArray); \
    GetNArray(v, ary); \
    sp = (short *)ary->ptr; \
}

#define Clint_to_NArray(v, rank, shape, lp) \
{ \
    struct NARRAY *ary; \
    v = na_make_object(NA_LINT, rank, shape, cNArray); \
    GetNArray(v, ary); \
    lp = (int *)ary->ptr; \
}
#define Cfloat_to_NArray(v, rank, shape, fp) \
{ \
    struct NARRAY *ary; \
    v = na_make_object(NA_SFLOAT, rank, shape, cNArray); \
    GetNArray(v, ary); \
    fp = (float *)ary->ptr; \
}
#define Cdouble_to_NArray(v, rank, shape, dp); \
{ \
    struct NARRAY *ary; \
    v = na_make_object(NA_DFLOAT, rank, shape, cNArray); \
    GetNArray(v, ary); \
    dp = (double *)ary->ptr; \
}

/* Array or NArray to pointer and length (with no new allocation) */

#define Array_to_Cfloat_len(obj, ptr, len) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_SFLOAT); \
    GetNArray(obj, na); \
    ptr = (float *) NA_PTR(na,0); \
    len = na->total; \
}

#define Array_to_Cfloat_len_shape(obj, ptr, len, shape) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_SFLOAT); \
    GetNArray(obj, na); \
    ptr = (float *) NA_PTR(na,0); \
    len = na->total; \
    shape = na->shape; \
}

#define Array_to_Cdouble_len(obj, ptr, len) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_DFLOAT); \
    GetNArray(obj, na); \
    ptr = (double *) NA_PTR(na,0); \
    len = na->total; \
}
#define Array_to_Cdouble_len_shape(obj, ptr, len, shape) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_DFLOAT); \
    GetNArray(obj, na); \
    ptr = (double *) NA_PTR(na,0); \
    len = na->total; \
    shape = na->shape; \
}

#define Array_to_Cbyte_len(obj, ptr, len) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_BYTE); \
    GetNArray(obj, na); \
    ptr = (u_int8_t *) NA_PTR(na,0); \
    len = na->total; \
}

#define Array_to_Cbyte_len_shape(obj, ptr, len, shape) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_BYTE); \
    GetNArray(obj, na); \
    ptr = (u_int8_t *) NA_PTR(na,0); \
    len = na->total; \
    shape = na->shape; \
}

#define Array_to_Csint_len(obj, ptr, len) \
{ \
  struct NARRAY *na; \
  obj = na_cast_object(obj, NA_SINT); \
  GetNArray(obj, na); \
  ptr = (int16_t *) NA_PTR(na,0); \
  len = na->total; \
}

#define Array_to_Csint_len_shape(obj, ptr, len, shape) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_SINT); \
    GetNArray(obj, na); \
    ptr = (int16_t *) NA_PTR(na,0); \
    len = na->total; \
    shape = na->shape; \
}


#define Array_to_Clint_len(obj, ptr, len) \
{ \
  struct NARRAY *na; \
  obj = na_cast_object(obj, NA_LINT); \
  GetNArray(obj, na); \
  ptr = (int32_t *) NA_PTR(na,0); \
  len = na->total; \
}

#define Array_to_Clint_len_shape(obj, ptr, len, shape) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_LINT); \
    GetNArray(obj, na); \
    ptr = (int32_t *) NA_PTR(na,0); \
    len = na->total; \
    shape = na->shape; \
}


/* Array or NArray to pointer (with no new allocation) */

#define Array_to_Cfloat(obj, ptr) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_SFLOAT); \
    GetNArray(obj, na); \
    ptr = (float *) NA_PTR(na,0); \
}
#define Array_to_Cdouble(obj, ptr) \
{ \
    struct NARRAY *na; \
    obj = na_cast_object(obj, NA_DFLOAT); \
    GetNArray(obj, na); \
    ptr = (double *) NA_PTR(na,0); \
}
#define Array_to_Cbyte(obj, ptr) \
{ \
  struct NARRAY *na; \
  obj = na_cast_object(obj, NA_BYTE); \
  GetNArray(obj, na); \
  ptr = (u_int8_t *) NA_PTR(na,0); \
}
#define Array_to_Csint(obj, ptr) \
{ \
  struct NARRAY *na; \
  obj = na_cast_object(obj, NA_SINT); \
  GetNArray(obj, na); \
  ptr = (int16_t *) NA_PTR(na,0); \
} 
#define Array_to_Clint(obj, ptr) \
{ \
  struct NARRAY *na; \
  obj = na_cast_object(obj, NA_LINT); \
  GetNArray(obj, na); \
  ptr = (int32_t *) NA_PTR(na,0); \
}

#define NC_RAISE(status) rb_raise(err_status2class(status),"%s",(nc_strerror(status)))
#define NC_RAISE2(status, str) rb_raise(err_status2class(status),"%s (%s)",nc_strerror(status),(str) )

static VALUE mNumRu = 0;
static VALUE cNetCDF;
static VALUE cNetCDFDim;
static VALUE cNetCDFAtt;
static VALUE cNetCDFVar;

static VALUE rb_eNetcdfError;
static VALUE rb_eNetcdfBadid;
static VALUE rb_eNetcdfNfile;
static VALUE rb_eNetcdfExist;
static VALUE rb_eNetcdfInval;
static VALUE rb_eNetcdfPerm;
static VALUE rb_eNetcdfNotindefine;
static VALUE rb_eNetcdfIndefine;
static VALUE rb_eNetcdfInvalcoords;
static VALUE rb_eNetcdfMaxdims;
static VALUE rb_eNetcdfNameinuse;
static VALUE rb_eNetcdfNotatt;
static VALUE rb_eNetcdfMaxatts;
static VALUE rb_eNetcdfBadtype;
static VALUE rb_eNetcdfBaddim;
static VALUE rb_eNetcdfUnlimpos;
static VALUE rb_eNetcdfMaxvars;
static VALUE rb_eNetcdfNotvar;
static VALUE rb_eNetcdfGlobal;
static VALUE rb_eNetcdfNotnc;
static VALUE rb_eNetcdfSts;
static VALUE rb_eNetcdfMaxname;
static VALUE rb_eNetcdfUnlimit;
static VALUE rb_eNetcdfNorecvars;
static VALUE rb_eNetcdfChar;
static VALUE rb_eNetcdfEdge;
static VALUE rb_eNetcdfStride;
static VALUE rb_eNetcdfBadname;
static VALUE rb_eNetcdfRange;
static VALUE rb_eNetcdfNomem;

/* Special Error */
/* Global error status */

static VALUE rb_eNetcdfFatal;

/* Global options variable. Used to determine behavior of error handler. */

static VALUE rb_eNetcdfEntool;
static VALUE rb_eNetcdfExdr;
static VALUE rb_eNetcdfSyserr;
 

struct Netcdf{
  int ncid;
  char *name;
  int closed;
};

struct NetCDFDim{
  int dimid;
  int ncid;
};

struct NetCDFVar{
  int varid;
  int ncid;
  VALUE file;
};

struct NetCDFAtt{
  int varid;
  int ncid;
  char *name;
};

static struct Netcdf *
NetCDF_init(int ncid,char *filename)
{
  struct Netcdf *Netcdffile;
  Netcdffile=xmalloc(sizeof(struct Netcdf));
  Netcdffile->ncid=ncid;
  Netcdffile->closed=0;
  Netcdffile->name=xmalloc((strlen(filename)+1)*sizeof(char));
  strcpy(Netcdffile->name,filename);  
  return(Netcdffile);
}

static struct NetCDFDim *
NetCDF_dim_init(int ncid,int dimid)
{
  struct NetCDFDim *Netcdf_dim;
  Netcdf_dim=xmalloc(sizeof(struct NetCDFDim));
  Netcdf_dim->dimid=dimid;
  Netcdf_dim->ncid=ncid;
  return(Netcdf_dim);
}

static struct NetCDFVar *
NetCDF_var_init(int ncid,int varid,VALUE file)
{
  struct NetCDFVar *Netcdf_var;
  Netcdf_var=xmalloc(sizeof(struct NetCDFVar));
  Netcdf_var->varid=varid;
  Netcdf_var->ncid=ncid;
  Netcdf_var->file=file;
  return(Netcdf_var);
}
static struct NetCDFAtt *
NetCDF_att_init(int ncid,int varid,char *attname)
{
  struct NetCDFAtt *Netcdf_att;
  Netcdf_att=xmalloc(sizeof(struct NetCDFAtt));
  Netcdf_att->ncid=ncid;
  Netcdf_att->varid=varid;
  Netcdf_att->name=xmalloc((strlen(attname)+1)*sizeof(char));
  strcpy(Netcdf_att->name,attname);   
  return(Netcdf_att);
}

void
Netcdf_att_free(struct NetCDFAtt *Netcdf_att)
{
  free(Netcdf_att->name);
  free(Netcdf_att);
}

void
NetCDF_var_free(struct NetCDFVar *Netcdf_var)
{
  free(Netcdf_var);
}

void
NetCDF_dim_free(struct NetCDFDim *Netcdf_dim)
{
  free(Netcdf_dim);
}

void
NetCDF_free(struct Netcdf *Netcdffile)
{
  if (!Netcdffile->closed){
      nc_close(Netcdffile->ncid); /* no error check -- not to stop during GC */
  }
  free(Netcdffile->name); 
  free(Netcdffile);
}

static VALUE
err_status2class(int status)
{
  if(NC_ISSYSERR(status)){
    return(rb_eNetcdfSyserr);
  }
  switch(status)
    {
    case(NC_EBADID):
      return(rb_eNetcdfBadid);break;
    case(NC_ENFILE):
      return(rb_eNetcdfNfile);break;
    case(NC_EEXIST):
      return(rb_eNetcdfExist);break;
    case(NC_EINVAL):
      return(rb_eNetcdfInval);break;
    case(NC_EPERM):
      return(rb_eNetcdfPerm);break;
    case(NC_ENOTINDEFINE):
      return(rb_eNetcdfNotindefine);break;
    case(NC_EINDEFINE):
      return(rb_eNetcdfIndefine);break;
    case(NC_EINVALCOORDS):
      return(rb_eNetcdfInvalcoords);break;
    case(NC_EMAXDIMS):
      return(rb_eNetcdfMaxdims);break;
    case(NC_ENAMEINUSE):
      return(rb_eNetcdfNameinuse);break;
    case(NC_ENOTATT):
      return(rb_eNetcdfNotatt);break;
    case(NC_EMAXATTS):
      return(rb_eNetcdfMaxatts);break;
    case(NC_EBADTYPE):
      return(rb_eNetcdfBadtype);break;
    case(NC_EBADDIM):
      return(rb_eNetcdfBaddim);break;
    case(NC_EUNLIMPOS):
      return(rb_eNetcdfUnlimpos);break;
    case(NC_EMAXVARS):
      return(rb_eNetcdfMaxvars);break;
    case(NC_ENOTVAR):
      return(rb_eNetcdfNotvar);break;
    case(NC_EGLOBAL):
      return(rb_eNetcdfGlobal);break;
    case(NC_ENOTNC):
      return(rb_eNetcdfNotnc);break;
    case(NC_ESTS):
      return(rb_eNetcdfSts);break;
    case(NC_EMAXNAME):
      return(rb_eNetcdfMaxname);break;
    case(NC_EUNLIMIT):
      return(rb_eNetcdfUnlimit);break;
    case(NC_ENORECVARS):
      return(rb_eNetcdfNorecvars);break;
    case(NC_ECHAR):
      return(rb_eNetcdfChar);break;
    case(NC_EEDGE):
      return(rb_eNetcdfEdge);break;
    case(NC_ESTRIDE):
      return(rb_eNetcdfStride);break;
    case(NC_EBADNAME):
      return(rb_eNetcdfBadname);break;
    case(NC_ERANGE):
      return(rb_eNetcdfRange);break;
    case(NC_ENOMEM):
      return(rb_eNetcdfNomem);break;
      /* case(NC_ENTOOL):
	 return(rb_eNetcdfEntool);break; */
    case(NC_EXDR):
      return(rb_eNetcdfExdr);break;
    case(NC_SYSERR):
      return(rb_eNetcdfSyserr);break;
    case(NC_FATAL):
      return(rb_eNetcdfFatal);break;
    }
  return rb_eNetcdfError;
}

static const char*
nctype2natype(int nctype){
  switch(nctype){
  case NC_CHAR:
    return("char");
  case NC_BYTE:
    return("byte");
  case NC_SHORT:
    return("sint");
  case NC_INT:
    return("int");
  case NC_FLOAT:
    return("sfloat");
  case NC_DOUBLE:
    return("float");
  default:
    rb_raise(rb_eNetcdfError, "No such netcdf type number %d\n",nctype);
  }
}

static int
nctype2natypecode(int nctype){
  switch(nctype){
  case NC_CHAR:
    return(NA_BYTE);
  case NC_BYTE:
    return(NA_BYTE);
  case NC_SHORT:
    return(NA_SINT);
  case NC_INT:
    return(NA_LINT);
  case NC_FLOAT:
    return(NA_SFLOAT);
  case NC_DOUBLE:
    return(NA_DFLOAT);
  default:
    rb_raise(rb_eNetcdfError, "No such netcdf type number %d\n",nctype);
  }
}

static int
natype2nctype(char *natype)
{
  if(strcmp(natype,"byte")==0) return(NC_BYTE);
  else if(strcmp(natype,"char")==0) return(NC_CHAR);
  else if(strcmp(natype,"text")==0) return(NC_CHAR);    /* alias of char */
  else if(strcmp(natype,"string")==0) return(NC_CHAR);  /* alias of char */
  else if(strcmp(natype,"sint")==0) return(NC_SHORT);
  else if(strcmp(natype,"int")==0) return(NC_INT);
  else if(strcmp(natype,"sfloat")==0) return(NC_FLOAT);
  else if(strcmp(natype,"float")==0) return(NC_DOUBLE);
  else rb_raise(rb_eNetcdfError, "No such NArray type '%s'",natype);
}

static int
natypecode2nctype(int natypecode)
{
  if(natypecode==NA_BYTE) return(NC_BYTE);
  else if(natypecode==NA_SINT) return(NC_SHORT);
  else if(natypecode==NA_LINT) return(NC_INT);
  else if(natypecode==NA_SFLOAT) return(NC_FLOAT);
  else if(natypecode==NA_DFLOAT) return(NC_DOUBLE);
  else rb_raise(rb_eNetcdfError, "No such NArray typecode '%d'",natypecode);
}

static void
nc_mark_obj(struct NetCDFVar *netcdf_var)
{
    VALUE ptr;

    ptr = netcdf_var->file;
    rb_gc_mark(ptr);
}


VALUE
NetCDF_clone(VALUE file)
{
    VALUE clone;
    struct Netcdf *nc1, *nc2;

    Data_Get_Struct(file, struct Netcdf, nc1);
    nc2 = NetCDF_init(nc1->ncid, nc1->name);
    clone = Data_Wrap_Struct(cNetCDF, 0, NetCDF_free, nc2);
    CLONESETUP(clone, file);
    return clone;
}

VALUE
NetCDF_dim_clone(VALUE dim)
{
    VALUE clone;
    struct NetCDFDim *nd1, *nd2;

    Data_Get_Struct(dim, struct NetCDFDim, nd1);
    nd2 = NetCDF_dim_init(nd1->ncid, nd1->dimid);
    clone = Data_Wrap_Struct(cNetCDFDim, 0, NetCDF_dim_free, nd2);
    CLONESETUP(clone, dim);
    return clone;
}

VALUE
NetCDF_att_clone(VALUE att)
{
    VALUE clone;
    struct NetCDFAtt *na1, *na2;

    Data_Get_Struct(att, struct NetCDFAtt, na1);
    na2 = NetCDF_att_init(na1->ncid, na1->varid, na1->name);
    clone = Data_Wrap_Struct(cNetCDFAtt, 0, Netcdf_att_free, na2);
    CLONESETUP(clone, att);
    return clone;
}

VALUE
NetCDF_var_clone(VALUE var)
{
    VALUE clone;
    struct NetCDFVar *nv1, *nv2;

    Data_Get_Struct(var, struct NetCDFVar, nv1);
    nv2 = NetCDF_var_init(nv1->ncid, nv1->varid, nv1->file);
    clone = Data_Wrap_Struct(cNetCDFVar, nc_mark_obj, NetCDF_var_free, nv2);
    CLONESETUP(clone, var);
    return clone;
}

VALUE
NetCDF_inq_libvers(VALUE mod)
{
  VALUE str;
  str = rb_str_new2(nc_inq_libvers());
  return(str);
}

VALUE
NetCDF_close(file)
     VALUE file;
{
  int status;
  int ncid;
  struct Netcdf *Netcdffile;

  if (rb_safe_level() >= 3 && !OBJ_TAINTED(file)) {
      rb_raise(rb_eSecurityError, "Insecure: can't close");
  }
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  if(!Netcdffile->closed){
      status = nc_close(ncid);
      if(status != NC_NOERR) NC_RAISE(status); 
      Netcdffile->closed = 1;
  } else {
      rb_warn("file %s is already closed", Netcdffile->name);
  }
  return Qnil;
}

VALUE
NetCDF_def_dim(VALUE file,VALUE dim_name,VALUE length)
{
  char* c_dim_name;
  size_t c_length;
  int ncid;
  int dimidp;
  int status;
  struct Netcdf *Netcdffile;
  struct NetCDFDim *Netcdf_dim;
  VALUE Dimension;
  
  rb_secure(3);
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  
  Check_Type(dim_name,T_STRING);
  c_dim_name=RSTRING_PTR(dim_name);
  c_length=NUM2UINT(length);
  ncid=Netcdffile->ncid;

  status = nc_def_dim(ncid,c_dim_name,c_length,&dimidp);
  if(status !=NC_NOERR) NC_RAISE(status);

  Netcdf_dim = NetCDF_dim_init(ncid,dimidp);

  Dimension = Data_Wrap_Struct(cNetCDFDim,0,NetCDF_dim_free,Netcdf_dim);
  return Dimension;
}


static VALUE
NetCDF_put_att_char(int ncid, char *name,VALUE value,VALUE atttype, int varid)
{
    int status;
    struct NetCDFAtt *ncatt;

    /* check atttype (not necessarily needed but it's better to do it) */
    if (TYPE(atttype) == T_STRING){
	if ( natype2nctype(RSTRING_PTR(atttype)) !=  NC_CHAR ) {
	    rb_raise(rb_eNetcdfError,
	        "attribute type must be 'char' (or nil) for a String value");
	}
    } else if (TYPE(atttype) != T_NIL) {
	rb_raise(rb_eNetcdfError,
		 "type specfication must be by a string or nil");
    }
    /* put value */
    Check_Type(value,T_STRING);
    status = nc_put_att_text(ncid, varid, name,
			     RSTRING_LEN(value), RSTRING_PTR(value));
    if(status != NC_NOERR) NC_RAISE(status);

    ncatt = NetCDF_att_init(ncid,varid,name);
    return (Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,ncatt));
}

static VALUE
NetCDF_put_att_numeric(int ncid, char *name,VALUE value,VALUE atttype, int varid)
{
    VALUE val;
    struct NARRAY *na_val;
    int na_typecode, status, len;
    char *ptr;
    struct NetCDFAtt *ncatt;

    /* check atttype and cast to an appropriate NArray if needed */

    if (TYPE(atttype) != T_NIL){
	na_typecode = na_get_typecode(atttype);
	GetNArray( na_cast_object(value, na_typecode), na_val );
    } else {
	if (TYPE(value)==T_ARRAY) {
	    val = RARRAY_PTR(value)[0]; /* to check the 1st elemnt if Array */
	} else {
	    val = value;
	}
	switch(TYPE(val)){
	case T_FIXNUM:
	case T_BIGNUM:
	    na_typecode = NA_LINT;
	    GetNArray( na_cast_object(value, na_typecode), na_val );
	    break;
	case T_FLOAT:
	    na_typecode = NA_DFLOAT;
	    GetNArray( na_cast_object(value, na_typecode), na_val );
	    break;
	case T_DATA:
	    if ( IsNArray(value) ){
		GetNArray(value,na_val);
		na_typecode = na_val->type;
	    } else {
		rb_raise(rb_eNetcdfError,"value has a wrong data type");
	    }
	    break;
	default:
	    rb_raise(rb_eNetcdfError,
		     "value (or its first element) has a wrong type");
	}
    }

    /* put value */

    len = na_val->total;
    ptr = na_val->ptr;
    switch(na_typecode){
    case NA_BYTE: 
	status = nc_put_att_uchar(ncid,varid,name,NC_BYTE,len,(unsigned char *)ptr);
	break;
    case NA_SINT: 
	status = nc_put_att_short(ncid,varid,name,NC_SHORT,len,(short *)ptr);
	break;
    case NA_LINT: 
	status = nc_put_att_int(ncid,varid,name,NC_INT,len,(int *)ptr);
	break;
    case NA_SFLOAT: 
	status = nc_put_att_float(ncid,varid,name,NC_FLOAT,len,(float *)ptr);
	break;
    case NA_DFLOAT: 
	status = nc_put_att_double(ncid,varid,name,NC_DOUBLE,len,(double*)ptr);
	break;
    default:
	rb_raise(rb_eNetcdfError,
		 "unsupported type. code = %d",na_typecode);
    }
    if(status != NC_NOERR) NC_RAISE(status);

    ncatt = NetCDF_att_init(ncid,varid,name);
    return (Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,ncatt));
}

static VALUE
NetCDF_put_att__(int ncid, char *name, VALUE value, VALUE atttype, int varid)
     /*
      * atttype: nil or a String ("string","int",etc). If nil,
      *          the type of attribute is determined from the type of value
      */
{
    switch(TYPE(value)){
    case T_STRING:
	return(NetCDF_put_att_char(ncid, name, value, atttype, varid));
    default:
	return(NetCDF_put_att_numeric(ncid, name, value, atttype, varid));
    }
}

VALUE
NetCDF_put_att(VALUE file,VALUE att_name,VALUE value,VALUE atttype)
     /*
      * atttype: nil or a String ("string","int",etc). If nil,
      *          the type of attribute is determined from the type of value
      */
{
    struct Netcdf *ncfile;
    char *name;

    rb_secure(3);
    Data_Get_Struct(file,struct Netcdf,ncfile);
    Check_Type(att_name,T_STRING);
    name = RSTRING_PTR(att_name);

    return( NetCDF_put_att__(ncfile->ncid, name, value, atttype, NC_GLOBAL) );
}

VALUE
NetCDF_put_att_var(VALUE var,VALUE att_name,VALUE value,VALUE atttype)
     /*
      * atttype: nil or a String ("string","int",etc). If nil,
      *          the type of attribute is determined from the type of value
      */
{
    struct NetCDFVar *ncvar;
    char *name;

    rb_secure(3);
    Data_Get_Struct(var,struct NetCDFVar,ncvar);
    Check_Type(att_name,T_STRING);
    name = RSTRING_PTR(att_name);

    return( NetCDF_put_att__(ncvar->ncid, name, value, atttype, ncvar->varid));
}


VALUE
NetCDF_def_var(VALUE file,VALUE var_name,VALUE vartype,VALUE dimensions)
{
  int ncid;
  char *c_var_name;
  static int xtype;
  long c_ndims;
  int varidp;
  int dimidp;
  int i=0;
  int status;
  char *c_dim_name;
  int c_dimids[NC_MAX_DIMS];
  struct Netcdf *Netcdffile;
  struct NetCDFVar *Netcdf_var;
  struct NetCDFDim *Netcdf_dim;
  VALUE Var;

  rb_secure(3);
  Check_Type(var_name,T_STRING);
  Check_Type(dimensions,T_ARRAY);

  c_var_name=RSTRING_PTR(var_name);
  c_ndims=RARRAY_LEN(dimensions);
  
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;

  if (TYPE(vartype) == T_STRING){
      xtype = natype2nctype(RSTRING_PTR(vartype));     
  } else if (TYPE(vartype) == T_FIXNUM){
      xtype = natypecode2nctype(NUM2INT(vartype));     
  } else {
      rb_raise(rb_eNetcdfError,
	       "type specfication must be by a string or nil");
  }

  for(i=0;i<c_ndims;i++){
    switch(TYPE(RARRAY_PTR(dimensions)[c_ndims-1-i])){
    case T_STRING:
      Check_Type(RARRAY_PTR(dimensions)[c_ndims-1-i],T_STRING);
      c_dim_name=StringValueCStr(RARRAY_PTR(dimensions)[c_ndims-1-i]);
      status=nc_inq_dimid(ncid,c_dim_name,&dimidp);
      if(status != NC_NOERR) NC_RAISE(status);
      c_dimids[i]=dimidp;
      break;
    case T_DATA:
      Data_Get_Struct(RARRAY_PTR(dimensions)[c_ndims-1-i],struct NetCDFDim,Netcdf_dim);
      c_dimids[i]=Netcdf_dim->dimid;
      break;
    default:
      rb_raise(rb_eNetcdfError, "No such object of the netCDF dimension class.");
    }
  }
  
  status = nc_def_var(ncid,c_var_name,xtype,c_ndims,c_dimids,&varidp);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Netcdf_var = NetCDF_var_init(ncid,varidp,file);
  
  Var=Data_Wrap_Struct(cNetCDFVar,nc_mark_obj,NetCDF_var_free,Netcdf_var);
  return Var;
}

VALUE
NetCDF_dim(VALUE file,VALUE dim_name)
{
  int ncid;
  char *c_dim_name;
  int dimidp;
  int status;
  struct Netcdf *Netcdffile;
  struct NetCDFDim *Netcdf_dim;
  VALUE Dimension;

  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  Check_Type(dim_name,T_STRING);
  c_dim_name=RSTRING_PTR(dim_name);
  
  status = nc_inq_dimid(ncid,c_dim_name,&dimidp);
  if(status !=NC_NOERR){
    if(status == NC_EBADDIM){ 
      return(Qnil);  /*2003/08/27 back to orig (from changes on 2003/02/03)*/
    } else{
      NC_RAISE(status);
    }
  }

  Netcdf_dim=NetCDF_dim_init(ncid,dimidp);
  
  Dimension = Data_Wrap_Struct(cNetCDFDim,0,NetCDF_dim_free,Netcdf_dim);
  return Dimension;
}

VALUE  
NetCDF_var(VALUE file,VALUE var_name)
{  
  int ncid;
  int status;
  int varidp;
  char *c_var_name;
  struct Netcdf *Netcdffile;
  struct NetCDFVar *Netcdf_var;
  VALUE Variable;

  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  Check_Type(var_name,T_STRING);
  c_var_name=RSTRING_PTR(var_name);
  
  status=nc_inq_varid(ncid,c_var_name,&varidp);
  if(status != NC_NOERR){
    if(status == NC_ENOTVAR){
      return(Qnil); /*2003/08/27 back to orig (from changes on 2003/02/03)*/
    } else{ 
      NC_RAISE(status);
    }
  }
  
  Netcdf_var = NetCDF_var_init(ncid,varidp,file);
  Variable = Data_Wrap_Struct(cNetCDFVar,nc_mark_obj,NetCDF_var_free,Netcdf_var);
  return Variable;
}

VALUE
NetCDF_att(VALUE file,VALUE att_name)
{
  int ncid;
  int status;
  int attnump;
  char *c_att_name;
  struct Netcdf *Netcdffile;
  struct NetCDFAtt *Netcdf_att;
  VALUE Attribute;

  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  Check_Type(att_name,T_STRING);
  c_att_name=RSTRING_PTR(att_name);
  

  status = nc_inq_attid(ncid,NC_GLOBAL,c_att_name,&attnump);
  if(status != NC_NOERR){
    if(status == NC_ENOTATT){
      return(Qnil);
    }
    else{
      NC_RAISE(status);
    }
  }
  
  Netcdf_att = NetCDF_att_init(ncid,NC_GLOBAL,c_att_name);
  
  Attribute = Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,Netcdf_att);
   
  return Attribute;
}
VALUE
NetCDF_fill(VALUE file,VALUE mode)
{
  int ncid;
  int status;
  struct Netcdf *Netcdffile;
  int old_modep;
  
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid = Netcdffile->ncid;
  if(mode==Qfalse){
    status = nc_set_fill(ncid,NC_NOFILL,&old_modep);
    if(status != NC_NOERR) NC_RAISE(status);
  }
  else if(mode == Qtrue){
    status = nc_set_fill(ncid,NC_FILL,&old_modep);
    if(status != NC_NOERR) NC_RAISE(status);
  }
  else
    rb_raise(rb_eNetcdfError,"Usage:self.fill(true) or self.fill(false)");
  return Qnil;
}

VALUE
NetCDF_redef(VALUE file)
{
  int ncid;
  int status;
  struct Netcdf *Netcdffile;

  rb_secure(3);
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  status = nc_redef(ncid);
  if(status !=NC_NOERR){
    if(status == NC_EINDEFINE){
      return Qnil;
    }
    else{
      NC_RAISE(status);
    }
  }
  return Qtrue;
}

VALUE 
NetCDF_enddef(VALUE file)
{
  int ncid;
  int status;
  struct Netcdf *Netcdffile;

  rb_secure(3);
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  status = nc_enddef(ncid);
  if(status !=NC_NOERR){ 
    if(status == NC_ENOTINDEFINE){
      return Qnil;
    }
    else{
      NC_RAISE(status);
    }
  }
  return Qtrue;
}

VALUE 
NetCDF_whether_in_define_mode(VALUE file)
{
  /* returns true if the NetCDF object is currently in the define mode,
             false if in the data mode, and
	     nil if else (possibly the file is read-only, or some other
	     error occurred)
  */
  int ncid;
  int status;
  struct Netcdf *Netcdffile;

  rb_secure(3);
  Data_Get_Struct(file,struct Netcdf,Netcdffile);
  ncid=Netcdffile->ncid;
  status = nc_redef(ncid);
  if(status == NC_EINDEFINE){
    return Qtrue;
  } else if(status == NC_NOERR) {
    /* was in the data mode --> recover the data mode and report false */
    status = nc_enddef(ncid);
    if(status == NC_NOERR) {
      return Qfalse;
    } else {
      return Qnil;
    }
  } else {
    return Qnil;
  }
}

VALUE
NetCDF_open(VALUE mod,VALUE filename,VALUE omode)     
{
  int status;
  int ncid;
  char* c_filename;
  int c_omode;
  struct Netcdf *ncfile;
  VALUE retval;

  Check_Type(filename,T_STRING);
  SafeStringValue(filename);
  c_filename=RSTRING_PTR(filename);
  Check_Type(omode,T_FIXNUM);
  c_omode=NUM2INT(omode);
  
  status = nc_open(c_filename,c_omode,&ncid);
  if(status !=NC_NOERR){NC_RAISE2(status,c_filename);}

  ncfile = NetCDF_init(ncid,c_filename);
  retval = Data_Wrap_Struct(cNetCDF,0,NetCDF_free,ncfile);
  return( retval );
}

VALUE
NetCDF_create(VALUE mod,VALUE filename,VALUE cmode)
{
  int ncid;
  int status;
  char* c_filename;
  int c_cmode;
  struct Netcdf *ncfile;
  
  Check_Type(filename,T_STRING);
  SafeStringValue(filename);
  c_filename=RSTRING_PTR(filename);
  Check_Type(cmode,T_FIXNUM);
  c_cmode=NUM2INT(cmode);
  
  status = nc_create(c_filename,c_cmode,&ncid);
  if(status != NC_NOERR) NC_RAISE2(status, c_filename);

  ncfile = NetCDF_init(ncid,c_filename);
  return( Data_Wrap_Struct(cNetCDF,0,NetCDF_free,ncfile) );
}

VALUE 
NetCDF_ndims(VALUE file)
{
  int ncid;
  int ndimsp;
  VALUE Integer;
  int status;
  struct Netcdf *ncfile;

  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  status = nc_inq_ndims(ncid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE (status);
  Integer = INT2NUM(ndimsp);
  return Integer;
}

VALUE 
NetCDF_nvars(VALUE file)
{
  int ncid;
  int nvarsp;
  int status;
  VALUE Integer;
  struct Netcdf *ncfile;
  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  status = nc_inq_nvars(ncid,&nvarsp);
  if(status != NC_NOERR) NC_RAISE (status);
  Integer = INT2NUM(nvarsp);
  return Integer;
}

VALUE 
NetCDF_natts(VALUE file)
{
  int ncid;
  int nattsp;
  int status;
  VALUE Integer;
  struct Netcdf *ncfile;

  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  status=nc_inq_natts(ncid,&nattsp);
  if(status != NC_NOERR) NC_RAISE (status);
  Integer = INT2NUM(nattsp);
  return Integer;
}

VALUE
NetCDF_unlimited(VALUE file)
{
  int ncid;
  int unlimdimidp;
  int status;
  struct Netcdf *ncfile;
  struct NetCDFDim *Netcdf_dim;
  VALUE Dimension;

  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  status=nc_inq_unlimdim(ncid,&unlimdimidp);
  if(status !=NC_NOERR) NC_RAISE(status);
  
  Netcdf_dim = NetCDF_dim_init(ncid,unlimdimidp);
  
  /* If unlimdimidp=-1,No unlimited dimension is defined in the netCDF dataset */ 
  if(unlimdimidp != -1)
    {
      Dimension = Data_Wrap_Struct(cNetCDFDim,0,NetCDF_dim_free,Netcdf_dim);
      return Dimension;
    }
  else
    {
      return Qnil;
    }
}
			       
VALUE
NetCDF_sync(VALUE file)
{
  int ncid;
  int status;
  struct Netcdf *ncfile;

  rb_secure(3);
  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  status = nc_sync(ncid);
  if(status !=NC_NOERR) NC_RAISE (status);
  return Qnil;
}

VALUE
NetCDF_path(VALUE file)
{
  char *path;
  struct Netcdf *ncfile;
  
  Data_Get_Struct(file,struct Netcdf,ncfile);
  path=ncfile->name;
  return(rb_str_new2(path));
}

VALUE
NetCDF_dim_length(VALUE Dim)
{
  int ncid;
  int status;
  int dimid;
  size_t lengthp;
  struct NetCDFDim *Netcdf_dim;
  
  Data_Get_Struct(Dim,struct NetCDFDim,Netcdf_dim);
  ncid=Netcdf_dim->ncid;
  dimid=Netcdf_dim->dimid;

  status = nc_inq_dimlen(ncid,dimid,&lengthp);
  if(status != NC_NOERR) NC_RAISE(status);
  
  return(INT2NUM(lengthp));
}

VALUE
NetCDF_dim_name(VALUE Dim,VALUE dimension_newname)
{
  int ncid;
  int status;
  int dimid;
  char *c_dim_name;
  struct NetCDFDim *Netcdf_dim;
  
  rb_secure(3);
  Data_Get_Struct(Dim,struct NetCDFDim,Netcdf_dim);
  ncid=Netcdf_dim->ncid;
  dimid=Netcdf_dim->dimid;
  Check_Type(dimension_newname,T_STRING);
  c_dim_name = StringValueCStr(dimension_newname);

  status = nc_rename_dim(ncid,dimid,c_dim_name);
  if(status !=NC_NOERR) NC_RAISE(status);
  
  return Qnil;
}

VALUE
NetCDF_dim_inqname(VALUE Dim)
{
  int ncid;
  int status;
  int dimid;
  char c_dim_name[NC_MAX_NAME];
  struct NetCDFDim *Netcdf_dim;
  VALUE str;
  
  Data_Get_Struct(Dim,struct NetCDFDim,Netcdf_dim);
  ncid=Netcdf_dim->ncid;
  dimid=Netcdf_dim->dimid;
  
  status = nc_inq_dimname(ncid,dimid,c_dim_name);
  if(status !=NC_NOERR) NC_RAISE(status);
  
  str = rb_str_new2(c_dim_name);
  OBJ_TAINT(str);
  return(str);
}

VALUE
NetCDF_dim_whether_unlimited(VALUE Dim)
{
  int status;
  int uldid;
  struct NetCDFDim *Netcdf_dim;
  
  Data_Get_Struct(Dim,struct NetCDFDim,Netcdf_dim);
  status=nc_inq_unlimdim(Netcdf_dim->ncid,&uldid);
  if(status !=NC_NOERR) NC_RAISE(status);
  if(Netcdf_dim->dimid == uldid){
      return(Qtrue);
  } else {
      return(Qfalse);
  }
}

VALUE
NetCDF_att_inq_name(VALUE Att)
{
  char *c_att_name;
  struct NetCDFAtt *Netcdf_att;
  VALUE str;

  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);
  c_att_name=Netcdf_att->name;
  
  str = rb_str_new2(c_att_name);
  OBJ_TAINT(str);
  return(str);
}

VALUE
NetCDF_att_rename(VALUE Att,VALUE new_att_name)
{
  int ncid;
  int status;
  int varid;
  char *c_att_name;
  char *c_new_att_name;
  struct NetCDFAtt *Netcdf_att;
  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);
  ncid=Netcdf_att->ncid;
  varid=Netcdf_att->varid;
  
  c_att_name=Netcdf_att->name;
  
  Check_Type(new_att_name,T_STRING);
  SafeStringValue(new_att_name);
  c_new_att_name=StringValueCStr(new_att_name);
  
  status = nc_rename_att(ncid,varid,c_att_name,c_new_att_name);
  if(status != NC_NOERR) NC_RAISE(status);
  
  strcpy(Netcdf_att->name,c_new_att_name);
  return Qnil;
}

VALUE
NetCDF_id2dim(VALUE file,VALUE dimid)
{
  int ncid;
  int c_dimid;
  struct Netcdf *ncfile;
  struct NetCDFDim *Netcdf_dim;
  VALUE Dim;

  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  Check_Type(dimid,T_FIXNUM);
  c_dimid=NUM2INT(dimid);
  Netcdf_dim = NetCDF_dim_init(ncid,c_dimid);
  Dim=Data_Wrap_Struct(cNetCDFDim,0,NetCDF_dim_free,Netcdf_dim);
  return(Dim);
}

VALUE
NetCDF_id2var(VALUE file,VALUE varid)
{
  int ncid;
  int c_varid;
  struct Netcdf *ncfile;
  struct NetCDFVar *Netcdf_var;
  VALUE Var;
  
  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;
  Check_Type(varid,T_FIXNUM);
  c_varid=NUM2INT(varid);
  Netcdf_var = NetCDF_var_init(ncid,c_varid,file);
  Var=Data_Wrap_Struct(cNetCDFVar,nc_mark_obj,NetCDF_var_free,Netcdf_var);
  return(Var);
}


VALUE
NetCDF_id2att(VALUE file,VALUE attnum)
{
  int ncid;
  int c_attnum;
  int status;
  struct Netcdf *ncfile;
  struct NetCDFAtt *Netcdf_att;
  char *c_att_name;
  VALUE Att;
  c_att_name=ALLOCA_N(char,NC_MAX_NAME);
  
  Data_Get_Struct(file,struct Netcdf,ncfile);
  ncid=ncfile->ncid;

  Check_Type(attnum,T_FIXNUM);
  c_attnum=NUM2INT(attnum);

  status = nc_inq_attname(ncid,NC_GLOBAL,c_attnum,c_att_name);
  if(status != NC_NOERR) NC_RAISE(status);

  Netcdf_att=NetCDF_att_init(ncid,NC_GLOBAL,c_att_name);

  Att=Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,Netcdf_att);
  return(Att);

}

#if NCVER >= 400
/* USAGE
    NetCDFVar#deflate(deflate_level, shuffle=false)
 */
VALUE
NetCDF_var_deflate(int argc, VALUE *argv, VALUE Var)
{
  int ncid, varid, status;
  struct NetCDFVar *Netcdf_var;

  int shuffle;
    /* If non-zero, turn on the shuffle filter. 

       http://www.unidata.ucar.edu/software/netcdf/papers/AMS_2008.pdf :
       The shuffle algorithm changes the byte order in the data stream;
       when used with integers that are all close together, this
       results in a better compression ratio. There is no benefit
       from using the shuffle filter without also using
       compression.

       MEMO by horinouchi: shuffling filter was also effective for float
       variables in some test (demo5-netcdf4.rb).
     */
  int deflate_level;
  int deflate=1;
        /* Always set to non-zero:
           See https://www.unidata.ucar.edu/software/netcdf/docs/netcdf-c/nc_005fdef_005fvar_005fdeflate.html#nc_005fdef_005fvar_005fdeflate
           If non-zero, turn on the deflate filter at the
           level specified by the deflate_level parameter.
         */

  if (argc>2 || argc<1) rb_raise(rb_eArgError, 
		         "wrong # of arguments (%d). It must be 1 or 2", argc);

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;

  deflate_level = NUM2INT(argv[0]);

  if (argc==1) {
      shuffle = 0;  /* default: false */
  } else {
      if ( argv[1] == Qnil || argv[1] == Qfalse ) {
	  shuffle = 0;
      } else {
	  shuffle = 1;
      }
  }

  status = nc_def_var_deflate(ncid, varid, shuffle, deflate, deflate_level);
  if(status != NC_NOERR) NC_RAISE(status);

  return(Var);
}

VALUE
NetCDF_var_deflate_params(VALUE Var)
{
  int ncid, varid, status;
  struct NetCDFVar *Netcdf_var;
  int shufflep, deflatep, deflate_levelp;
  VALUE sh, df, params;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_var_deflate(ncid, varid, &shufflep, &deflatep, 
			      &deflate_levelp);
  if(status != NC_NOERR) NC_RAISE(status);
  if (shufflep==0) {sh=Qfalse;} else {sh=Qtrue;}
  if (deflatep==0) {df=Qfalse;} else {df=Qtrue;}
  params = rb_ary_new3(3, sh, df, INT2NUM(deflate_levelp) );
  return(params);
}

VALUE
NetCDF_var_set_endian(VALUE Var, VALUE endian)
{
  int ncid, varid, status;
  struct NetCDFVar *Netcdf_var;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_def_var_endian(ncid, varid, NUM2INT(endian));
  if(status != NC_NOERR) NC_RAISE(status);
  return(Var);
}

VALUE
NetCDF_var_endian(VALUE Var)
{
  int ncid, varid, status;
  struct NetCDFVar *Netcdf_var;
  int endian;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_var_endian(ncid, varid, &endian);
  if(status != NC_NOERR) NC_RAISE(status);
  return(INT2FIX(endian));
}

#endif

VALUE
NetCDF_var_id2att(VALUE Var,VALUE attnum)
{
  int ncid;
  int c_attnum;
  int status;
  int c_varid;
  struct NetCDFVar *Netcdf_var;
  struct NetCDFAtt *Netcdf_att;
  char *c_att_name;
  VALUE Att;
  
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  c_varid=Netcdf_var->varid;

  Check_Type(attnum,T_FIXNUM);
  c_attnum=NUM2INT(attnum);

  c_att_name=ALLOCA_N(char,NC_MAX_NAME);

  status = nc_inq_attname(ncid,c_varid,c_attnum,c_att_name);
  if(status != NC_NOERR) NC_RAISE(status);
 
  Netcdf_att=NetCDF_att_init(ncid,c_varid,c_att_name);
  Att=Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,Netcdf_att);
  return(Att);
}

VALUE
NetCDF_var_dims(VALUE Var)
{
  int ncid, *dimids, ndims, varid, i, status;
  struct NetCDFVar *Netcdf_var;
  struct NetCDFDim *Netcdf_dim;
  VALUE Dims;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  Dims = rb_ary_new();
  for(i=0;i<ndims;i++){
      Netcdf_dim = NetCDF_dim_init(ncid,dimids[ndims-1-i]);
      rb_ary_push(Dims, 
		  Data_Wrap_Struct(cNetCDFDim,0,NetCDF_dim_free,Netcdf_dim));
  }
  return(Dims);
}

VALUE
NetCDF_var_dim(VALUE Var, VALUE ith)
{
  int ncid, *dimids, ndims, varid, status, c_ith;
  struct NetCDFVar *Netcdf_var;
  struct NetCDFDim *Netcdf_dim;
  VALUE Dim;

  Check_Type(ith,T_FIXNUM);
  c_ith=NUM2INT(ith);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(c_ith < 0 || c_ith >= ndims) {
      rb_raise(rb_eNetcdfError, "dimension count less than zero or greater than ndims-1");
  }
  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  Netcdf_dim = NetCDF_dim_init(ncid,dimids[ndims-1-c_ith]);
  Dim = Data_Wrap_Struct(cNetCDFDim,0,NetCDF_dim_free,Netcdf_dim);
  return(Dim);
}

VALUE
NetCDF_att_copy(VALUE Att,VALUE Var_or_File)
{
  int ncid_in,ncid_out;
  int status;
  int varid_in,varid_out;
  char *att_name;
  struct NetCDFAtt *Netcdf_att;
  struct NetCDFVar *Netcdf_var;
  struct Netcdf    *ncfile;
  struct NetCDFAtt *Netcdf_att_out;

  rb_secure(3);
  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);
  ncid_in=Netcdf_att->ncid;
  varid_in=Netcdf_att->varid;
  att_name=Netcdf_att->name;

  if( rb_obj_is_kind_of(Var_or_File, cNetCDFVar) ){
      Data_Get_Struct(Var_or_File,struct NetCDFVar, Netcdf_var);
      ncid_out=Netcdf_var->ncid;
      varid_out=Netcdf_var->varid;
  } else if ( rb_obj_is_kind_of(Var_or_File, cNetCDF) ){
      Data_Get_Struct(Var_or_File,struct Netcdf, ncfile);
      ncid_out=ncfile->ncid;
      varid_out=NC_GLOBAL;
  } else {
      rb_raise(rb_eNetcdfError,"The argument must be a NetCDFVar or a NetCDF");
  }

  status = nc_copy_att(ncid_in,varid_in,att_name,ncid_out,varid_out);
  if(status != NC_NOERR) NC_RAISE(status);
  Netcdf_att_out = NetCDF_att_init(ncid_out,varid_out,att_name);
  return (Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,Netcdf_att_out));
}

VALUE
NetCDF_att_atttype(VALUE Att)
{
  int ncid;
  int varid;
  int status;
  char *att_name;
  const char *Attname;
  struct NetCDFAtt *Netcdf_att;
  nc_type xtypep;

  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);
  ncid = Netcdf_att->ncid;
  varid = Netcdf_att->varid;
  att_name = Netcdf_att->name;

  status = nc_inq_atttype(ncid,varid,att_name,&xtypep);
  if(status != NC_NOERR) NC_RAISE(status);

  Attname = nctype2natype(xtypep);
  return(rb_str_new2(Attname));
}
  
VALUE
NetCDF_att_typecode(VALUE Att)
{
  int ncid;
  int varid;
  int status;
  char *att_name;
  struct NetCDFAtt *Netcdf_att;
  nc_type xtypep;

  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);
  ncid = Netcdf_att->ncid;
  varid = Netcdf_att->varid;
  att_name = Netcdf_att->name;

  status = nc_inq_atttype(ncid,varid,att_name,&xtypep);
  if(status != NC_NOERR) NC_RAISE(status);
  
  return(INT2NUM(nctype2natypecode(xtypep)));
}
  
VALUE
NetCDF_att_delete(VALUE Att)
{
  int ncid;
  int status;
  int varid;
  char *c_att_name;
  struct NetCDFAtt *Netcdf_att;

  rb_secure(3);
  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);

  ncid=Netcdf_att->ncid;
  varid=Netcdf_att->varid;
  c_att_name=Netcdf_att->name;

  status = nc_del_att(ncid,varid,c_att_name);
  if(status != NC_NOERR) NC_RAISE(status);
  
  return Qnil;
}

VALUE
NetCDF_att_put(VALUE Att,VALUE value,VALUE atttype)
     /*
      * atttype: nil or a String ("string","int",etc). If nil,
      *          the type of attribute is determined from the type of value
      */
{
  struct NetCDFAtt *ncatt;
 
  rb_secure(3);
  Data_Get_Struct(Att,struct NetCDFAtt,ncatt);
  return( NetCDF_put_att__(ncatt->ncid, ncatt->name, value, 
			   atttype, ncatt->varid) );
}
  
VALUE
NetCDF_att_get(VALUE Att)
{
  int ncid;
  int varid;
  char *c_attname;
  int status;
  struct NetCDFAtt *Netcdf_att;
  nc_type xtypep;
  size_t lenp;
  na_shape_t attlen[1];    /* NArray uses int instead of size_t */
  char *tp;
  unsigned char *up;
  short *sp;
  int *ip;
  float *fp;
  double *dp;
  VALUE NArray;
  VALUE str;

  Data_Get_Struct(Att,struct NetCDFAtt,Netcdf_att);
  ncid = Netcdf_att->ncid;
  varid = Netcdf_att->varid;
  c_attname = Netcdf_att->name;

  status = nc_inq_atttype(ncid,varid,c_attname,&xtypep);
  if(status != NC_NOERR) NC_RAISE(status);

  switch(xtypep){
  case NC_CHAR:
    status = nc_inq_attlen(ncid,varid,c_attname,&lenp);
    if(status != NC_NOERR) NC_RAISE(status);
    tp = ALLOCA_N(char,lenp+1);
    tp[lenp]= '\0';
    status = nc_get_att_text(ncid,varid,c_attname,tp);
    if(status != NC_NOERR) NC_RAISE(status);
    str = rb_str_new2(tp);
    OBJ_TAINT(str);
    return(str);
    break;
  case NC_BYTE:
    status = nc_inq_attlen(ncid,varid,c_attname,&lenp);
    if(status != NC_NOERR) NC_RAISE(status);

    attlen[0]=lenp;
    Cbyte_to_NArray(NArray,1,attlen,up);
  
    status = nc_get_att_uchar(ncid,varid,c_attname,up);
    if(status != NC_NOERR) NC_RAISE(status);

    OBJ_TAINT(NArray);
    return NArray;
    break;
  case NC_SHORT:
    status = nc_inq_attlen(ncid,varid,c_attname,&lenp);
    if(status != NC_NOERR) NC_RAISE(status);

    attlen[0]=lenp;
    Csint_to_NArray(NArray,1,attlen,sp);
    
    status = nc_get_att_short(ncid,varid,c_attname,sp);
    if(status != NC_NOERR) NC_RAISE(status);
    OBJ_TAINT(NArray);
    return NArray;
    break;
  case NC_INT:
    status = nc_inq_attlen(ncid,varid,c_attname,&lenp);
    if(status != NC_NOERR) NC_RAISE(status);
    
    attlen[0]=lenp;
    Clint_to_NArray(NArray,1,attlen,ip);
    
    status = nc_get_att_int(ncid,varid,c_attname,ip);
    if(status != NC_NOERR) NC_RAISE(status);

    OBJ_TAINT(NArray);
    return NArray;
    break;
  case NC_FLOAT:
    status = nc_inq_attlen(ncid,varid,c_attname,&lenp);
    if(status != NC_NOERR) NC_RAISE(status);

    attlen[0]=lenp;
    Cfloat_to_NArray(NArray,1,attlen,fp);
    
    status = nc_get_att_float(ncid,varid,c_attname,fp);
    if(status != NC_NOERR) NC_RAISE(status);

    OBJ_TAINT(NArray);
    return NArray;
    break;
  case NC_DOUBLE:
    status = nc_inq_attlen(ncid,varid,c_attname,&lenp);
    if(status != NC_NOERR) NC_RAISE(status);

    attlen[0]=lenp;
    Cdouble_to_NArray(NArray,1,attlen,dp);
    
    status = nc_get_att_double(ncid,varid,c_attname,dp);
    if(status != NC_NOERR) NC_RAISE(status);
    OBJ_TAINT(NArray);
    return NArray;
    break;
  default:
    rb_raise(rb_eNetcdfError,"atttype isn't supported in netCDF");
  }
  return Qnil;
}

		  
VALUE
NetCDF_var_inq_name(VALUE Var)
{
  int ncid;
  int status;
  int varid;
  char c_var_name[NC_MAX_NAME];
  struct NetCDFVar *Netcdf_var;
  VALUE Var_name;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);

  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varname(ncid,varid,c_var_name);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Var_name=rb_str_new2(c_var_name);
  OBJ_TAINT(Var_name);
  return Var_name;
}

VALUE
NetCDF_var_ndims(VALUE Var)
{
  int ncid;
  int status;
  int varid;
  int ndimsp;
  struct NetCDFVar *Netcdf_var;
  VALUE Var_ndims;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  Var_ndims=INT2FIX(ndimsp);
  return Var_ndims;
}

VALUE
NetCDF_var_vartype(VALUE Var)
{
  int ncid;
  int status;
  int varid;
  nc_type xtypep;
  struct NetCDFVar *Netcdf_var;
  const char *Vartype;
  
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  status = nc_inq_vartype(ncid,varid,&xtypep);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Vartype=nctype2natype(xtypep);
  return(rb_str_new2(Vartype));
}

VALUE
NetCDF_var_typecode(VALUE Var)
{
  int ncid;
  int status;
  int varid;
  nc_type xtypep;
  struct NetCDFVar *Netcdf_var;
  
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  status = nc_inq_vartype(ncid,varid,&xtypep);
  if(status != NC_NOERR) NC_RAISE(status);
  
  return(INT2NUM(nctype2natypecode(xtypep)));
}

 
VALUE
NetCDF_var_natts(VALUE Var)
{
  int ncid;
  int status;
  int varid;
  int nattsp;
  struct NetCDFVar *Netcdf_var;
  VALUE Var_natts;
  

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  
  status= nc_inq_varnatts(ncid,varid,&nattsp);
  if(status !=NC_NOERR) NC_RAISE(status);
  
  Var_natts=INT2FIX(nattsp);
  return Var_natts;
}

VALUE
NetCDF_var_file(VALUE Var)
{
  struct NetCDFVar *Netcdf_var;
  /* VALUE file; */
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  
  return (Netcdf_var->file);
}

VALUE
NetCDF_var_rename(VALUE Var,VALUE var_new_name)
{
  int ncid;
  int status;
  int varid;
  char *c_var_new_name;
  struct NetCDFVar *Netcdf_var;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Check_Type(var_new_name,T_STRING);
  c_var_new_name=StringValueCStr(var_new_name);
  
  status = nc_rename_var(ncid,varid,c_var_new_name);
  if(status !=NC_NOERR) NC_RAISE(status);

  return Qnil;
}

VALUE
NetCDF_var_att(VALUE Var,VALUE att_name)
{
  int ncid;
  int status;
  int varid;
  char *c_att_name;
  int c_attnump;
  struct NetCDFVar *Netcdf_var;
  struct NetCDFAtt *Netcdf_att;
  VALUE Att;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Check_Type(att_name,T_STRING);
  c_att_name=StringValueCStr(att_name);
  
  status = nc_inq_attid(ncid,varid,c_att_name,&c_attnump);
  if(status == NC_NOERR){
    Netcdf_att=NetCDF_att_init(ncid,varid,c_att_name);
    Att=Data_Wrap_Struct(cNetCDFAtt,0,Netcdf_att_free,Netcdf_att);
    return Att;
  }
  else if(status == NC_ENOTATT){
    return Qnil;
  }
  else{
    NC_RAISE(status);
    return Qnil;
  }
}

/* Redifinition of the "==" and "eql?" methods */

VALUE 
NetCDF_eql(VALUE filea,VALUE fileb)
{
  struct Netcdf *ncfilea;
  struct Netcdf *ncfileb;
  
  if( rb_obj_is_kind_of(fileb, cNetCDF) ){
      Data_Get_Struct(filea,struct Netcdf,ncfilea);
      Data_Get_Struct(fileb,struct Netcdf,ncfileb);
    
      if(ncfilea->ncid == ncfileb->ncid && 
	 strcmp(ncfilea->name,ncfileb->name)==0){
	  return Qtrue;
      } else {
	  return Qfalse;
      }
  } else {
      return Qfalse;
  }
}

VALUE
NetCDF_var_eql(VALUE Vara,VALUE Varb)
{
  struct NetCDFVar *Netcdf_vara;
  struct NetCDFVar *Netcdf_varb;
  
  if( rb_obj_is_kind_of(Varb, cNetCDFVar) ){
      Data_Get_Struct(Vara,struct NetCDFVar,Netcdf_vara);
      Data_Get_Struct(Varb,struct NetCDFVar,Netcdf_varb);

      if(Netcdf_vara->ncid == Netcdf_varb->ncid && 
	 Netcdf_vara->varid == Netcdf_varb->varid){
	  return Qtrue;
      } else {
	  return Qfalse;
      }
  } else {
      return Qfalse;
  }
}

VALUE 
NetCDF_dim_eql(VALUE Dima,VALUE Dimb)
{
  struct NetCDFDim *Netcdf_dima;
  struct NetCDFDim *Netcdf_dimb;

  if( rb_obj_is_kind_of(Dimb, cNetCDFDim) ){
      Data_Get_Struct(Dima,struct NetCDFDim,Netcdf_dima);
      Data_Get_Struct(Dimb,struct NetCDFDim,Netcdf_dimb);
  
      if(Netcdf_dima->ncid == Netcdf_dimb->ncid && 
	 Netcdf_dima->dimid == Netcdf_dimb->dimid){
	  return Qtrue;
      } else {
	  return Qfalse;
      }
  } else {
      return Qfalse;
  }
}

VALUE
NetCDF_att_eql(VALUE Atta,VALUE Attb)
{
  struct NetCDFAtt *Netcdf_atta;
  struct NetCDFAtt *Netcdf_attb;
  
  if( rb_obj_is_kind_of(Attb, cNetCDFAtt) ){
      Data_Get_Struct(Atta,struct NetCDFAtt,Netcdf_atta);
      Data_Get_Struct(Attb,struct NetCDFAtt,Netcdf_attb);
  
      if(Netcdf_atta->ncid == Netcdf_atta->ncid && 
	 Netcdf_atta->varid == Netcdf_attb->varid && 
	 strcmp(Netcdf_atta->name,Netcdf_attb->name)==0){
	  return Qtrue;
      } else { 
	  return Qfalse;
      }
  } else {
      return Qfalse;
  }
}

/* Follow methods is to connect "NArray" with "Netcdf" */
VALUE
NetCDF_get_var_char(VALUE Var)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  struct NetCDFVar *Netcdf_var;
  int i=0;
  int ndimsp;
  int *dimids;
  size_t lengthp;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids = ALLOCA_N(int,ndimsp);
  if (ndimsp != 0){
      shape = ALLOCA_N(na_shape_t,ndimsp);
      for(i=0;i<ndimsp;i++){
	  status = nc_inq_vardimid(ncid,varid,dimids);
	  if(status != NC_NOERR) NC_RAISE(status);
	  nc_inq_dimlen(ncid,dimids[i],&lengthp);
	  shape[ndimsp-1-i]=lengthp;
      }
  } else {
      ndimsp = 1;
      shape = ALLOCA_N(na_shape_t,1);
      shape[0]=1;
  }

  Cbyte_to_NArray(NArray,ndimsp,shape,ptr);
  
  status = nc_get_var_text(ncid,varid,(char *)ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_var_byte(VALUE Var)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  struct NetCDFVar *Netcdf_var;
  int i=0;
  int ndimsp;
  int *dimids;
  size_t lengthp;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids = ALLOCA_N(int,ndimsp);
  if (ndimsp != 0){
      shape = ALLOCA_N(na_shape_t,ndimsp);
      for(i=0;i<ndimsp;i++){
	  status = nc_inq_vardimid(ncid,varid,dimids);
	  if(status != NC_NOERR) NC_RAISE(status);
	  nc_inq_dimlen(ncid,dimids[i],&lengthp);
	  shape[ndimsp-1-i]=lengthp;
      }
  } else {
      ndimsp = 1;
      shape = ALLOCA_N(na_shape_t,1);
      shape[0]=1;
  }

  Cbyte_to_NArray(NArray,ndimsp,shape,ptr);
  
  status = nc_get_var_uchar(ncid,varid,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_var_sint(VALUE Var)
{
  int ncid;
  int varid;
  int status;
  short *ptr;
  struct NetCDFVar *Netcdf_var;
  int i=0;
  int ndimsp;
  int *dimids;
  size_t lengthp;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids = ALLOCA_N(int,ndimsp);
  if (ndimsp != 0){
      shape = ALLOCA_N(na_shape_t,ndimsp);
      for(i=0;i<ndimsp;i++){
	  status = nc_inq_vardimid(ncid,varid,dimids);
	  if(status != NC_NOERR) NC_RAISE(status);
	  nc_inq_dimlen(ncid,dimids[i],&lengthp);
	  shape[ndimsp-1-i]=lengthp;
      }
  } else {
      ndimsp = 1;
      shape = ALLOCA_N(na_shape_t,1);
      shape[0]=1;
  }

  Csint_to_NArray(NArray,ndimsp,shape,ptr);
  
  status = nc_get_var_short(ncid,varid,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_var_int(VALUE Var)
{
  int ncid;
  int varid;
  int status;
  int *ptr;
  struct NetCDFVar *Netcdf_var;
  int i=0;
  int ndimsp;
  int *dimids;
  size_t lengthp;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids = ALLOCA_N(int,ndimsp);
  if (ndimsp != 0){
      shape = ALLOCA_N(na_shape_t,ndimsp);
      for(i=0;i<ndimsp;i++){
	  status = nc_inq_vardimid(ncid,varid,dimids);
	  if(status != NC_NOERR) NC_RAISE(status);
	  nc_inq_dimlen(ncid,dimids[i],&lengthp);
	  shape[ndimsp-1-i]=lengthp;
      }
  } else {
      ndimsp = 1;
      shape = ALLOCA_N(na_shape_t,1);
      shape[0]=1;
  }

  Clint_to_NArray(NArray,ndimsp,shape,ptr);
  
  status = nc_get_var_int(ncid,varid,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_var_float(VALUE Var)
{
  int ncid;
  int varid;
  int status;
  float *ptr;
  struct NetCDFVar *Netcdf_var;
  int i=0;
  int ndimsp;
  int *dimids;
  size_t lengthp;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids = ALLOCA_N(int,ndimsp);
  if (ndimsp != 0){
      shape = ALLOCA_N(na_shape_t,ndimsp);
      for(i=0;i<ndimsp;i++){
	  status = nc_inq_vardimid(ncid,varid,dimids);
	  if(status != NC_NOERR) NC_RAISE(status);
	  nc_inq_dimlen(ncid,dimids[i],&lengthp);
	  shape[ndimsp-1-i]=lengthp;
      }
  } else {
      ndimsp = 1;
      shape = ALLOCA_N(na_shape_t,1);
      shape[0]=1;
  }

  Cfloat_to_NArray(NArray,ndimsp,shape,ptr);

  status = nc_get_var_float(ncid,varid,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_var_double(VALUE Var)
{
  int ncid;
  int varid;
  int status;
  double *ptr;
  struct NetCDFVar *Netcdf_var;
  int i=0;
  int ndimsp;
  int *dimids;
  size_t lengthp;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  dimids = ALLOCA_N(int,ndimsp);
  if (ndimsp != 0){
      shape = ALLOCA_N(na_shape_t,ndimsp);
      for(i=0;i<ndimsp;i++){
	  status = nc_inq_vardimid(ncid,varid,dimids);
	  if(status != NC_NOERR) NC_RAISE(status);
	  nc_inq_dimlen(ncid,dimids[i],&lengthp);
	  shape[ndimsp-1-i]=lengthp;
      }
  } else {
      ndimsp = 1;
      shape = ALLOCA_N(na_shape_t,1);
      shape[0]=1;
  }

  Cdouble_to_NArray(NArray,ndimsp,shape,ptr);
  
  status = nc_get_var_double(ncid,varid,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE 
NetCDF_get_var1_char(VALUE Var,VALUE start)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int dimids[NC_MAX_DIMS];
  size_t dimlen;
  na_shape_t *c_count;
  VALUE NArray;

  
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR)NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }
  
  c_start=ALLOCA_N(size_t,ndims);
  c_count=ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    l_start = NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
    
    c_count[i]=1;
  }
  
  
  
  
  Cbyte_to_NArray(NArray,ndims,c_count,ptr);
  status = nc_get_var1_text(ncid,varid,c_start,(char *)ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;

}

VALUE 
NetCDF_get_var1_byte(VALUE Var,VALUE start)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int dimids[NC_MAX_DIMS];
  size_t dimlen;
  na_shape_t *c_count;
  VALUE NArray;

  
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR)NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }
  
  c_start=ALLOCA_N(size_t,ndims);
  c_count=ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    l_start = NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
    
    c_count[i]=1;
  }
  
  
  
  
  Cbyte_to_NArray(NArray,ndims,c_count,ptr);
  status = nc_get_var1_uchar(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;

}

VALUE 
NetCDF_get_var1_sint(VALUE Var,VALUE start)
{
  int ncid;
  int varid;
  int status;
  short *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int dimids[NC_MAX_DIMS];
  size_t dimlen;
  na_shape_t *c_count;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }
  
  c_start=ALLOCA_N(size_t,ndims);
  c_count=ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    l_start = NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
    c_count[i]=1;
  }
  
  Csint_to_NArray(NArray,ndims,c_count,ptr);
  
  status = nc_get_var1_short(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;

}

VALUE 
NetCDF_get_var1_int(VALUE Var,VALUE start)
{
  int ncid;
  int varid;
  int status;
  int *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int dimids[NC_MAX_DIMS];
  size_t dimlen;
  na_shape_t *c_count;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR)NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }
  
  c_start=ALLOCA_N(size_t,ndims);
  c_count=ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    l_start = NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
    c_count[i]=1;
  }
  
  Clint_to_NArray(NArray,ndims,c_count,ptr);
  
  status = nc_get_var1_int(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;

}

VALUE 
NetCDF_get_var1_float(VALUE Var,VALUE start)
{
  int ncid;
  int varid;
  int status;
  float *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int dimids[NC_MAX_DIMS];
  size_t dimlen;
  na_shape_t *c_count;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR)NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }
  
  c_start=ALLOCA_N(size_t,ndims);
  c_count=ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    l_start = NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    status = nc_inq_vardimid(ncid, varid, dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
    c_count[i]=1;
  }
  
  Cfloat_to_NArray(NArray,ndims,c_count,ptr);
  
  status = nc_get_var1_float(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;

}

VALUE 
NetCDF_get_var1_double(VALUE Var,VALUE start)
{
  int ncid;
  int varid;
  int status;
  double *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int dimids[NC_MAX_DIMS];
  size_t dimlen;
  na_shape_t *c_count;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR)NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }
  
  c_start=ALLOCA_N(size_t,ndims);
  c_count=ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    l_start = NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status !=NC_NOERR) NC_RAISE(status);
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
    c_count[i]=1;
  }
  
  Cdouble_to_NArray(NArray,ndims,c_count,ptr);
  
  status = nc_get_var1_double(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;

}

VALUE
NetCDF_get_vars_char(VALUE Var,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  int ndims;
  int *dimids;
  size_t dimlen;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }

  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims){
    rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n");
  }
  c_start = ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
   
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
  }

  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      c_stride[i]=1;
    }
    break;
  default:
    Check_Type(stride,T_ARRAY);
    if(RARRAY_LEN(stride) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'stride is too short\n");
    }
    for(i=0;i<ndims; i++){
      c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
      if(c_stride[i]==0){
	rb_raise(rb_eNetcdfError,"stride cannot be zero\n");
      }
    }
  }

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      nc_inq_dimlen(ncid,dimids[i],&dimlen);
      c_count[i]=(dimlen-c_start[i]-1)/c_stride[i]+1;
    }
    break;
  default:
    Check_Type(end,T_ARRAY);
    if(RARRAY_LEN(end) <ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n");
    }
    for(i=0; i<ndims; i++){
      l_end= NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
      if(l_end < 0) {
	status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_end +=dimlen;
      }
      c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
    }
  }

  
  shape = ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    shape[ndims-1-i]=c_count[i];
  }
  
  Cbyte_to_NArray(NArray,ndims,shape,ptr);
  
  status = nc_get_vars_text(ncid,varid,c_start,c_count,c_stride,(char *)ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_vars_byte(VALUE Var,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  int ndims;
  int *dimids;
  size_t dimlen;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }

  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims){
    rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n");
  }
  c_start = ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
   
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
  }

  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      c_stride[i]=1;
    }
    break;
  default:
    Check_Type(stride,T_ARRAY);
    if(RARRAY_LEN(stride) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'stride is too short\n");
    }
    for(i=0;i<ndims; i++){
      c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
      if(c_stride[i]==0){
	rb_raise(rb_eNetcdfError,"stride cannot be zero\n");
      }
    }
  }

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      nc_inq_dimlen(ncid,dimids[i],&dimlen);
      c_count[i]=(dimlen-c_start[i]-1)/c_stride[i]+1;
    }
    break;
  default:
    Check_Type(end,T_ARRAY);
    if(RARRAY_LEN(end) <ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n");
    }
    for(i=0; i<ndims; i++){
      l_end= NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
      if(l_end < 0) {
	status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_end +=dimlen;
      }
      c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
    }
  }

  
  shape = ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    shape[ndims-1-i]=c_count[i];
  }
  
  Cbyte_to_NArray(NArray,ndims,shape,ptr);
  
  status = nc_get_vars_uchar(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_vars_sint(VALUE Var,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  short *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  int ndims;
  int *dimids;
  size_t dimlen;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }

  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims){
    rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n");
  }
  c_start = ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
   
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
  }

  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      c_stride[i]=1;
    }
    break;
  default:
    Check_Type(stride,T_ARRAY);
    if(RARRAY_LEN(stride) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'stride is too short\n");
    }
    for(i=0;i<ndims; i++){
      c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
      if(c_stride[i]==0){
	rb_raise(rb_eNetcdfError,"stride cannot be zero\n");
      }
    }
  }

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      nc_inq_dimlen(ncid,dimids[i],&dimlen);
      c_count[i]=(dimlen-c_start[i]-1)/c_stride[i]+1;
    }
    break;
  default:
    Check_Type(end,T_ARRAY);
    if(RARRAY_LEN(end) <ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n");
    }
    for(i=0; i<ndims; i++){
      l_end= NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
      if(l_end < 0) {
	status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_end +=dimlen;
      }
      c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
    }
  }
  
  
  shape = ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    shape[ndims-1-i]=c_count[i];
  }

  Csint_to_NArray(NArray,ndims,shape,ptr);
  

  status = nc_get_vars_short(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  
  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_vars_int(VALUE Var,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  int *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  int ndims;
  int *dimids;
  size_t dimlen;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }

  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims){
    rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n");
  }
  c_start = ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
   
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
  }

  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      c_stride[i]=1;
    }
    break;
  default:
    Check_Type(stride,T_ARRAY);
    if(RARRAY_LEN(stride) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'stride is too short\n");
    }
    for(i=0;i<ndims; i++){
      c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
      if(c_stride[i]==0){
	rb_raise(rb_eNetcdfError,"stride cannot be zero\n");
      }
    }
  }

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      nc_inq_dimlen(ncid,dimids[i],&dimlen);
      c_count[i]=(dimlen-c_start[i]-1)/c_stride[i]+1;
    }
    break;
  default:
    Check_Type(end,T_ARRAY);
    if(RARRAY_LEN(end) <ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n");
    }
    for(i=0; i<ndims; i++){
      l_end= NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
      if(l_end < 0) {
	status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_end +=dimlen;
      }
      c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
    }
  }

  
  shape = ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    shape[ndims-1-i]=c_count[i];
  }

  Clint_to_NArray(NArray,ndims,shape,ptr);
  

  status = nc_get_vars_int(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_vars_float(VALUE Var,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  float *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  int ndims;
  int *dimids;
  size_t dimlen;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }

  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims){
    rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n");
  }
  c_start = ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
   
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
  }

  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      c_stride[i]=1;
    }
    break;
  default:
    Check_Type(stride,T_ARRAY);
    if(RARRAY_LEN(stride) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'stride is too short\n");
    }
    for(i=0;i<ndims; i++){
      c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
      if(c_stride[i]==0){
	rb_raise(rb_eNetcdfError,"stride cannot be zero\n");
      }
    }
  }

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      nc_inq_dimlen(ncid,dimids[i],&dimlen);
      c_count[i]=(dimlen-c_start[i]-1)/c_stride[i]+1;
    }
    break;
  default:
    Check_Type(end,T_ARRAY);
    if(RARRAY_LEN(end) <ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n");
    }
    for(i=0; i<ndims; i++){
      l_end= NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
      if(l_end < 0) {
	status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_end +=dimlen;
      }
      c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
    }
  }
  

  shape = ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    shape[ndims-1-i]=c_count[i];
  }
  
  Cfloat_to_NArray(NArray,ndims,shape,ptr);
  
  
  status = nc_get_vars_float(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}

VALUE
NetCDF_get_vars_double(VALUE Var,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  double *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  na_shape_t *shape;    /* NArray uses int instead of size_t */
  int ndims;
  int *dimids;
  size_t dimlen;
  VALUE NArray;

  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid = Netcdf_var->ncid;
  varid = Netcdf_var->varid;
  
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);
  if(ndims == 0) {
    rb_raise(rb_eNetcdfError,"Cannot specify a subset of a rank-0 scalar\n");
  }

  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims){
    rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n");
  }
  c_start = ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
   
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;
  }

  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      c_stride[i]=1;
    }
    break;
  default:
    Check_Type(stride,T_ARRAY);
    if(RARRAY_LEN(stride) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'stride is too short\n");
    }
    for(i=0;i<ndims; i++){
      c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
      if(c_stride[i]==0){
	rb_raise(rb_eNetcdfError,"stride cannot be zero\n");
      }
    }
  }

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
    for(i=0; i<ndims; i++){
      nc_inq_dimlen(ncid,dimids[i],&dimlen);
      c_count[i]=(dimlen-c_start[i]-1)/c_stride[i]+1;
    }
    break;
  default:
    Check_Type(end,T_ARRAY);
    if(RARRAY_LEN(end) <ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n");
    }
    for(i=0; i<ndims; i++){
      l_end= NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
      if(l_end < 0) {
	status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_end +=dimlen;
      }
      c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
    }
  }
  

  shape = ALLOCA_N(na_shape_t,ndims);
  for(i=0;i<ndims;i++){
    shape[ndims-1-i]=c_count[i];
  }
  
  Cdouble_to_NArray(NArray,ndims,shape,ptr);
  
  status = nc_get_vars_double(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);

  OBJ_TAINT(NArray);
  return NArray;
}


VALUE
NetCDF_put_var_char(VALUE Var,VALUE NArray)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr,scalar;
  na_shape_t len,i=0;
  struct NetCDFVar *Netcdf_var;
  na_shape_t nc_tlen=1;
  int ndimsp;
  int dimids[NC_MAX_DIMS];
  size_t lengthp;
  char *var_name;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Array_to_Cbyte_len(NArray,ptr,len);
  
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  for(i=0;i<ndimsp;i++){
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    nc_inq_dimlen(ncid,dimids[i],&lengthp);
    nc_tlen=lengthp*nc_tlen;
  }
  if(len == 1 && len != nc_tlen){
    scalar = *ptr;
    ptr = ALLOCA_N(unsigned char,nc_tlen);
    for(i=0;i<nc_tlen;i++){ptr[i]=scalar;}
  } else if(len != nc_tlen){
    var_name=ALLOCA_N(char,NC_MAX_NAME);
    status = nc_inq_varname(ncid,varid,var_name);
    if(status != NC_NOERR) NC_RAISE(status );
    rb_raise(rb_eNetcdfError,"Length of NArray don't equal to length of total array in the '%s'\n",var_name);
  }
  status = nc_put_var_text(ncid,varid,(char *)ptr);
  if(status !=NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var_byte(VALUE Var,VALUE NArray)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr,scalar;
  na_shape_t len,i=0;
  struct NetCDFVar *Netcdf_var;
  na_shape_t nc_tlen=1;
  int ndimsp;
  int dimids[NC_MAX_DIMS];
  size_t lengthp;
  char *var_name;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Array_to_Cbyte_len(NArray,ptr,len);
  
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  for(i=0;i<ndimsp;i++){
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    nc_inq_dimlen(ncid,dimids[i],&lengthp);
    nc_tlen=lengthp*nc_tlen;
  }
  if(len == 1 && len != nc_tlen){
    scalar = *ptr;
    ptr = ALLOCA_N(unsigned char,nc_tlen);
    for(i=0;i<nc_tlen;i++){ptr[i]=scalar;}
  } else if(len != nc_tlen){
    var_name=ALLOCA_N(char,NC_MAX_NAME);
    status = nc_inq_varname(ncid,varid,var_name);
    if(status != NC_NOERR) NC_RAISE(status );
    rb_raise(rb_eNetcdfError,"Length of NArray don't equal to length of total array in the '%s'\n",var_name);
  }
  status = nc_put_var_uchar(ncid,varid,ptr);
  if(status !=NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var_short(VALUE Var,VALUE NArray)
{
  int ncid;
  int varid;
  int status;
  short *ptr,scalar;
  na_shape_t len,i=0;
  struct NetCDFVar *Netcdf_var;
  na_shape_t nc_tlen=1;
  int ndimsp;
  int dimids[NC_MAX_DIMS];
  size_t lengthp;
  char *var_name;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  Array_to_Csint_len(NArray,ptr,len);
  
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  for(i=0;i<ndimsp;i++){
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    nc_inq_dimlen(ncid,dimids[i],&lengthp);
    nc_tlen=lengthp*nc_tlen;
  }
  if(len == 1 && len != nc_tlen){
    scalar = *ptr;
    ptr = ALLOCA_N(short,nc_tlen);
    for(i=0;i<nc_tlen;i++){ptr[i]=scalar;}
  } else if(len != nc_tlen){
    var_name=ALLOCA_N(char,NC_MAX_NAME);
    status = nc_inq_varname(ncid,varid,var_name);
    if(status != NC_NOERR) NC_RAISE(status);
    rb_raise(rb_eNetcdfError,"Length of NArray don't equal to length of total array length in the '%s'\n",var_name);
  }

  status = nc_put_var_short(ncid,varid,ptr);
  if(status !=NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var_int(VALUE Var,VALUE NArray)
{
  int ncid;
  int varid;
  int status;
  int *ptr,scalar;
  na_shape_t len,i=0;
  struct NetCDFVar *Netcdf_var;
  na_shape_t nc_tlen=1;
  int ndimsp;
  int dimids[NC_MAX_DIMS];
  size_t lengthp;
  char *var_name;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Array_to_Clint_len(NArray,ptr,len);

  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  for(i=0;i<ndimsp;i++){
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    nc_inq_dimlen(ncid,dimids[i],&lengthp);
    nc_tlen=lengthp*nc_tlen;
  }
  if(len == 1 && len != nc_tlen){
    scalar = *ptr;
    ptr = ALLOCA_N(int,nc_tlen);
    for(i=0;i<nc_tlen;i++){ptr[i]=scalar;}
  } else if(len != nc_tlen){
    var_name=ALLOCA_N(char,NC_MAX_NAME);
    status = nc_inq_varname(ncid,varid,var_name);
    if(status != NC_NOERR) NC_RAISE(status);
    rb_raise(rb_eNetcdfError,"Length of NArray don't equal to length of total array length in the '%s'\n",var_name);
  }
  
  
  status = nc_put_var_int(ncid,varid,ptr);
  if(status !=NC_NOERR) NC_RAISE(status);
  return Qnil;
}


VALUE
NetCDF_put_var_float(VALUE Var,VALUE NArray)
{
  int ncid;
  int varid;
  int status;
  float *ptr,scalar;
  na_shape_t len,i=0;
  struct NetCDFVar *Netcdf_var;
  na_shape_t nc_tlen=1;
  int ndimsp;
  int dimids[NC_MAX_DIMS];
  size_t lengthp;
  char *var_name;
  
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Array_to_Cfloat_len(NArray,ptr,len);
  
  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  for(i=0;i<ndimsp;i++){
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    nc_inq_dimlen(ncid,dimids[i],&lengthp);
    nc_tlen=lengthp*nc_tlen;
  }
  if(len == 1 && len != nc_tlen){
    scalar = *ptr;
    ptr = ALLOCA_N(float,nc_tlen);
    for(i=0;i<nc_tlen;i++){ptr[i]=scalar;}
  } else if(len != nc_tlen){
    var_name=ALLOCA_N(char,NC_MAX_NAME);
    status = nc_inq_varname(ncid,varid,var_name);
    if(status != NC_NOERR) NC_RAISE(status);
    rb_raise(rb_eNetcdfError,"Length of NArray don't equal to length of total array length in the '%s'\n",var_name);
  }

  status = nc_put_var_float(ncid,varid,ptr);
  if(status !=NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var_double(VALUE Var,VALUE NArray)
{
  int ncid;
  int varid;
  int status;
  double *ptr,scalar;
  na_shape_t len,i=0;
  struct NetCDFVar *Netcdf_var;
  na_shape_t nc_tlen=1;
  int ndimsp;
  int dimids[NC_MAX_DIMS];
  size_t lengthp;
  char *var_name;


  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;

  Array_to_Cdouble_len(NArray,ptr,len);

  status = nc_inq_varndims(ncid,varid,&ndimsp);
  if(status != NC_NOERR) NC_RAISE(status);
  for(i=0;i<ndimsp;i++){
    status = nc_inq_vardimid(ncid,varid,dimids);
    if(status != NC_NOERR) NC_RAISE(status);
    nc_inq_dimlen(ncid,dimids[i],&lengthp);
    nc_tlen=lengthp*nc_tlen;
  }
  if(len == 1 && len != nc_tlen){
    scalar = *ptr;
    ptr = ALLOCA_N(double,nc_tlen);
    for(i=0;i<nc_tlen;i++){ptr[i]=scalar;}
  } else if(len != nc_tlen){
    var_name=ALLOCA_N(char,NC_MAX_NAME);
    status = nc_inq_varname(ncid,varid,var_name);
    if(status != NC_NOERR) NC_RAISE(status);
    rb_raise(rb_eNetcdfError,"Length of NArray don't equal to length of total array length in the '%s'\n",var_name);
  }

  status = nc_put_var_double(ncid,varid,ptr);
  if(status !=NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var1_char(VALUE Var,VALUE NArray,VALUE start)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int   *dimids;
  size_t dimlen;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  
  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) <ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }

  c_start=ALLOCA_N(size_t,ndims);
  for(i=0;i<ndims;i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;

  }
  Array_to_Cbyte(NArray,ptr);

  status = nc_put_var1_text(ncid,varid,c_start,(char *)ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var1_byte(VALUE Var,VALUE NArray,VALUE start)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int   *dimids;
  size_t dimlen;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  
  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) <ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }

  c_start=ALLOCA_N(size_t,ndims);
  for(i=0;i<ndims;i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;

  }
  Array_to_Cbyte(NArray,ptr);

  status = nc_put_var1_uchar(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var1_sint(VALUE Var,VALUE NArray,VALUE start)
{
  int ncid;
  int varid;
  int status;
  short *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int   *dimids;
  size_t dimlen;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  
  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) <ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }

  c_start=ALLOCA_N(size_t,ndims);
  for(i=0;i<ndims;i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;

  }
  Array_to_Csint(NArray,ptr);

  status = nc_put_var1_short(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}
VALUE
NetCDF_put_var1_int(VALUE Var,VALUE NArray,VALUE start)
{
  int ncid;
  int varid;
  int status;
  int *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int   *dimids;
  size_t dimlen;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  
  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) <ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }

  c_start=ALLOCA_N(size_t,ndims);
  for(i=0;i<ndims;i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;

  }
  Array_to_Clint(NArray,ptr);

  status = nc_put_var1_int(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var1_float(VALUE Var,VALUE NArray,VALUE start)
{
  int ncid;
  int varid;
  int status;
  float *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int   *dimids;
  size_t dimlen;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  
  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) <ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }

  c_start=ALLOCA_N(size_t,ndims);
  for(i=0;i<ndims;i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;

  }
  Array_to_Cfloat(NArray,ptr);

  status = nc_put_var1_float(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_var1_double(VALUE Var,VALUE NArray,VALUE start)
{
  int ncid;
  int varid;
  int status;
  double *ptr;
  int i;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start;
  size_t *c_start;
  int ndims;
  int   *dimids;
  size_t dimlen;
  
  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid,varid,&ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  
  dimids = ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid,varid,dimids);
  if(status != NC_NOERR) NC_RAISE(status);
  
  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) <ndims) {
    rb_raise(rb_eNetcdfError,"Length of 'start' is too short\n");
  }

  c_start=ALLOCA_N(size_t,ndims);
  for(i=0;i<ndims;i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
      status = nc_inq_dimlen(ncid,dimids[i],&dimlen);
      if(status != NC_NOERR) NC_RAISE(status);
      l_start += dimlen;
    }
    c_start[i]=l_start;

  }
  Array_to_Cdouble(NArray,ptr);

  status = nc_put_var1_double(ncid,varid,c_start,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}


VALUE
NetCDF_put_vars_char(VALUE Var,VALUE NArray,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr,scalar;
  na_shape_t len;
  int c_count_all=1;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  int ndims,i;
  na_shape_t *shape;
  int   *dimids;
  size_t dimlen;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid, varid, dimids);
  if(status != NC_NOERR) NC_RAISE(status);

  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n"); 
  }
  c_start=ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
	status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_start += dimlen;
    }
    c_start[i]=l_start;
  }
  
  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_stride[i]=1;
      }
      break;
  default:
      Check_Type(stride,T_ARRAY);
      if(RARRAY_LEN(stride) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'stride' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
	  if(c_stride[i]==0) {
	      rb_raise(rb_eNetcdfError, "stride cannot be zero\n"); 
	  }
      }
  }

  Array_to_Cbyte_len_shape(NArray,ptr,len,shape);

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_count[i]=shape[i];
      }
      c_count_all=len;
      break;
  default:
      Check_Type(end,T_ARRAY);
      if(RARRAY_LEN(end) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  l_end=NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
	  if(l_end < 0) {
	      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	      if(status != NC_NOERR) NC_RAISE(status);
	      l_end += dimlen;
	  }
	  c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
	  c_count_all=c_count[i]*c_count_all;
      }
      if(len == 1 && len != c_count_all){
	  scalar = *ptr;
	  ptr = ALLOCA_N(unsigned char,c_count_all);
	  for(i=0;i<c_count_all;i++){ptr[i]=scalar;}
      } else if(len != c_count_all) {
	rb_raise(rb_eNetcdfError, 
		 "lengh of the array does not agree with that of the subset\n"); 
      } 
  } 
  
  status = nc_put_vars_text(ncid,varid,c_start,c_count,c_stride,(char *)ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_vars_byte(VALUE Var,VALUE NArray,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  unsigned char *ptr,scalar;
  na_shape_t len;
  int c_count_all=1;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  int ndims,i;
  na_shape_t *shape;
  int   *dimids;
  size_t dimlen;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid, varid, dimids);
  if(status != NC_NOERR) NC_RAISE(status);

  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n"); 
  }
  c_start=ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
	status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_start += dimlen;
    }
    c_start[i]=l_start;
  }
  
  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_stride[i]=1;
      }
      break;
  default:
      Check_Type(stride,T_ARRAY);
      if(RARRAY_LEN(stride) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'stride' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
	  if(c_stride[i]==0) {
	      rb_raise(rb_eNetcdfError, "stride cannot be zero\n"); 
	  }
      }
  }

  Array_to_Cbyte_len_shape(NArray,ptr,len,shape);

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_count[i]=shape[i];
      }
      c_count_all=len;
      break;
  default:
      Check_Type(end,T_ARRAY);
      if(RARRAY_LEN(end) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  l_end=NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
	  if(l_end < 0) {
	      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	      if(status != NC_NOERR) NC_RAISE(status);
	      l_end += dimlen;
	  }
	  c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
	  c_count_all=c_count[i]*c_count_all;
      }
      if(len == 1 && len != c_count_all){
	  scalar = *ptr;
	  ptr = ALLOCA_N(unsigned char,c_count_all);
	  for(i=0;i<c_count_all;i++){ptr[i]=scalar;}
      } else if(len != c_count_all) {
	rb_raise(rb_eNetcdfError, 
		 "lengh of the array does not agree with that of the subset\n"); 
      } 
  } 
  
  status = nc_put_vars_uchar(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

VALUE
NetCDF_put_vars_sint(VALUE Var,VALUE NArray,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  short *ptr,scalar;
  na_shape_t len;
  int c_count_all=1;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  int ndims,i;
  na_shape_t *shape;
  int   *dimids;
  size_t dimlen;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid, varid, dimids);
  if(status != NC_NOERR) NC_RAISE(status);

  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n"); 
  }
  c_start=ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
	status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_start += dimlen;
    }
    c_start[i]=l_start;
  }
  
  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_stride[i]=1;
      }
      break;
  default:
      Check_Type(stride,T_ARRAY);
      if(RARRAY_LEN(stride) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'stride' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
	  if(c_stride[i]==0) {
	      rb_raise(rb_eNetcdfError, "stride cannot be zero\n"); 
	  }
      }
  }

  Array_to_Csint_len_shape(NArray,ptr,len,shape);

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_count[i]=shape[i];
      }
      c_count_all=len;
      break;
  default:
      Check_Type(end,T_ARRAY);
      if(RARRAY_LEN(end) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  l_end=NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
	  if(l_end < 0) {
	      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	      if(status != NC_NOERR) NC_RAISE(status);
	      l_end += dimlen;
	  }
	  c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
	  c_count_all=c_count[i]*c_count_all;
      }
      if(len == 1 && len != c_count_all){
	  scalar = *ptr;
	  ptr = ALLOCA_N(short,c_count_all);
	  for(i=0;i<c_count_all;i++){ptr[i]=scalar;}
      } else if(len != c_count_all) {
	  rb_raise(rb_eNetcdfError, 
              "lengh of the array does not agree with that of the subset\n"); 
      }
  }

  status = nc_put_vars_short(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}


VALUE
NetCDF_put_vars_int(VALUE Var,VALUE NArray,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  int *ptr,scalar;
  na_shape_t len;
  int c_count_all=1;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  int ndims,i;
  na_shape_t *shape;
  int   *dimids;
  size_t dimlen;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid, varid, dimids);
  if(status != NC_NOERR) NC_RAISE(status);

  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n"); 
  }
  c_start=ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
	status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_start += dimlen;
    }
    c_start[i]=l_start;
  }
  
  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_stride[i]=1;
      }
      break;
  default:
      Check_Type(stride,T_ARRAY);
      if(RARRAY_LEN(stride) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'stride' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
	  if(c_stride[i]==0) {
	      rb_raise(rb_eNetcdfError, "stride cannot be zero\n"); 
	  }
      }
  }

  Array_to_Clint_len_shape(NArray,ptr,len,shape);

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_count[i]=shape[i];
      }
      c_count_all=len;
      break;
  default:
      Check_Type(end,T_ARRAY);
      if(RARRAY_LEN(end) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  l_end=NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
	  if(l_end < 0) {
	      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	      if(status != NC_NOERR) NC_RAISE(status);
	      l_end += dimlen;
	  }
	  c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
	  c_count_all=c_count[i]*c_count_all;
      }
      if(len == 1 && len != c_count_all){
	  scalar = *ptr;
	  ptr = ALLOCA_N(int,c_count_all);
	  for(i=0;i<c_count_all;i++){ptr[i]=scalar;}
      } else if(len != c_count_all) {
	  rb_raise(rb_eNetcdfError, 
              "length of the array does not agree with that of the subset\n"); 
      }
  }

  status = nc_put_vars_int(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}


VALUE
NetCDF_put_vars_float(VALUE Var,VALUE NArray,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  float *ptr,scalar;
  na_shape_t len;
  int c_count_all=1;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  int ndims,i;
  na_shape_t *shape;
  int   *dimids;
  size_t dimlen;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid, varid, dimids);
  if(status != NC_NOERR) NC_RAISE(status);

  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n"); 
  }
  c_start=ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
	status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_start += dimlen;
    }
    c_start[i]=l_start;
  }
  
  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_stride[i]=1;
      }
      break;
  default:
      Check_Type(stride,T_ARRAY);
      if(RARRAY_LEN(stride) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'stride' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
	  if(c_stride[i]==0) {
	      rb_raise(rb_eNetcdfError, "stride cannot be zero\n"); 
	  }
      }
  }

  Array_to_Cfloat_len_shape(NArray,ptr,len,shape);

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_count[i]=shape[i];
      }
      c_count_all=len;
      break;
  default:
      Check_Type(end,T_ARRAY);
      if(RARRAY_LEN(end) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  l_end=NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
	  if(l_end < 0) {
	      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	      if(status != NC_NOERR) NC_RAISE(status);
	      l_end += dimlen;
	  }
	  c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
	  c_count_all=c_count[i]*c_count_all;
      }
      if(len == 1 && len != c_count_all){
	  scalar = *ptr;
	  ptr = ALLOCA_N(float,c_count_all);
	  for(i=0;i<c_count_all;i++){ptr[i]=scalar;}
      } else if(len != c_count_all) {
	  rb_raise(rb_eNetcdfError, 
              "lengh of the array does not agree with that of the subset\n"); 
      }
  }

  status = nc_put_vars_float(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}


VALUE
NetCDF_put_vars_double(VALUE Var,VALUE NArray,VALUE start,VALUE end,VALUE stride)
{
  int ncid;
  int varid;
  int status;
  double *ptr,scalar;
  na_shape_t len;
  int c_count_all=1;
  struct NetCDFVar *Netcdf_var;
  na_shape_t l_start, l_end;
  size_t *c_start;
  size_t *c_count;
  ptrdiff_t *c_stride;
  int ndims,i;
  na_shape_t *shape;
  int   *dimids;
  size_t dimlen;

  rb_secure(3);
  Data_Get_Struct(Var,struct NetCDFVar,Netcdf_var);
  ncid=Netcdf_var->ncid;
  varid=Netcdf_var->varid;
  status = nc_inq_varndims(ncid, varid, &ndims);
  if(status != NC_NOERR) NC_RAISE(status);

  dimids=ALLOCA_N(int,ndims);
  status = nc_inq_vardimid(ncid, varid, dimids);
  if(status != NC_NOERR) NC_RAISE(status);

  Check_Type(start,T_ARRAY);
  if(RARRAY_LEN(start) < ndims) {
      rb_raise(rb_eNetcdfError, "Length of 'start' is too short\n"); 
  }
  c_start=ALLOCA_N(size_t,ndims);
  for(i=0; i<ndims; i++){
    l_start=NUM2INT(RARRAY_PTR(start)[ndims-1-i]);
    
    if(l_start < 0) {
	status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	if(status != NC_NOERR) NC_RAISE(status);
	l_start += dimlen;
    }
    c_start[i]=l_start;
  }
  
  c_stride=ALLOCA_N(ptrdiff_t,ndims);
  switch(TYPE(stride)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_stride[i]=1;
      }
      break;
  default:
      Check_Type(stride,T_ARRAY);
      if(RARRAY_LEN(stride) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'stride' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  c_stride[i]=NUM2INT(RARRAY_PTR(stride)[ndims-1-i]);
	  if(c_stride[i]==0) {
	      rb_raise(rb_eNetcdfError, "stride cannot be zero\n"); 
	  }
      }
  }

  Array_to_Cdouble_len_shape(NArray,ptr,len,shape);

  c_count=ALLOCA_N(size_t,ndims);
  switch(TYPE(end)){
  case T_NIL:
      for(i=0; i<ndims; i++){
	  c_count[i]=shape[i];
      }
      c_count_all=len;
      break;
  default:
      Check_Type(end,T_ARRAY);
      if(RARRAY_LEN(end) < ndims) {
	  rb_raise(rb_eNetcdfError, "Length of 'end' is too short\n"); 
      }
      for(i=0; i<ndims; i++){
	  l_end=NUM2INT(RARRAY_PTR(end)[ndims-1-i]);
	  if(l_end < 0) {
	      status = nc_inq_dimlen(ncid, dimids[i], &dimlen);
	      if(status != NC_NOERR) NC_RAISE(status);
	      l_end += dimlen;
	  }
	  c_count[i]=(l_end-c_start[i])/c_stride[i]+1;
	  c_count_all=c_count[i]*c_count_all;
      }
      if(len == 1 && len != c_count_all){
	  scalar = *ptr;
	  ptr = ALLOCA_N(double,c_count_all);
	  for(i=0;i<c_count_all;i++){ptr[i]=scalar;}
      } else if(len != c_count_all) {
	  rb_raise(rb_eNetcdfError, 
              "lengh of the array does not agree with that of the subset\n"); 
      }
  }

  status = nc_put_vars_double(ncid,varid,c_start,c_count,c_stride,ptr);
  if(status != NC_NOERR) NC_RAISE(status);
  return Qnil;
}

void
Init_netcdfraw(void)
{
  mNumRu = rb_define_module("NumRu");

  /* Difinitions of the classes */
  cNetCDF = rb_define_class_under(mNumRu, "NetCDF", rb_cObject);
  cNetCDFDim = rb_define_class_under(mNumRu, "NetCDFDim", rb_cObject);
  cNetCDFAtt = rb_define_class_under(mNumRu, "NetCDFAtt", rb_cObject);
  cNetCDFVar = rb_define_class_under(mNumRu, "NetCDFVar", rb_cObject);

  rb_eNetcdfError = rb_define_class("NetcdfError",rb_eStandardError);
  rb_eNetcdfBadid = rb_define_class("NetcdfBadid",rb_eNetcdfError);
  rb_eNetcdfNfile = rb_define_class("NetcdfNfile",rb_eNetcdfError);
  rb_eNetcdfExist = rb_define_class("NetcdfExist",rb_eNetcdfError);
  rb_eNetcdfInval = rb_define_class("NetcdfInval",rb_eNetcdfError);
  rb_eNetcdfPerm = rb_define_class("NetcdfPerm",rb_eNetcdfError);
  rb_eNetcdfNotindefine = rb_define_class("NetcdfNotindefine",rb_eNetcdfError);
  rb_eNetcdfIndefine = rb_define_class("NetcdfIndefine",rb_eNetcdfError);
  rb_eNetcdfInvalcoords = rb_define_class("NetcdfInvalcoords",rb_eNetcdfError);
  rb_eNetcdfMaxdims = rb_define_class("NetcdfMaxdims",rb_eNetcdfError);
  rb_eNetcdfNameinuse = rb_define_class("NetcdfNameinuse",rb_eNetcdfError);
  rb_eNetcdfNotatt = rb_define_class("NetcdfNotatt",rb_eNetcdfError);
  rb_eNetcdfMaxatts = rb_define_class("NetcdfMaxatts",rb_eNetcdfError);
  rb_eNetcdfBadtype = rb_define_class("NetcdfBadtype",rb_eNetcdfError);
  rb_eNetcdfBaddim = rb_define_class("NetcdfBaddim",rb_eNetcdfError);
  rb_eNetcdfUnlimpos = rb_define_class("NetcdFUnlimpos",rb_eNetcdfError);
  rb_eNetcdfMaxvars = rb_define_class("NetcdfMaxvars",rb_eNetcdfError);
  rb_eNetcdfNotvar = rb_define_class("NetcdfNotvar",rb_eNetcdfError);
  rb_eNetcdfGlobal = rb_define_class("NetcdfGlobal",rb_eNetcdfError);
  rb_eNetcdfNotnc = rb_define_class("NetcdfNotnc",rb_eNetcdfError);
  rb_eNetcdfSts = rb_define_class("NetcdfSts",rb_eNetcdfError);
  rb_eNetcdfMaxname = rb_define_class("NetcdfMaxname",rb_eNetcdfError);
  rb_eNetcdfUnlimit = rb_define_class("NetcdfUnlimit",rb_eNetcdfError);
  rb_eNetcdfNorecvars = rb_define_class("NetcdfNorecvars",rb_eNetcdfError);
  rb_eNetcdfChar = rb_define_class("NetcdfChar",rb_eNetcdfError);
  rb_eNetcdfEdge = rb_define_class("NetcdfEdge",rb_eNetcdfError);
  rb_eNetcdfStride = rb_define_class("NetcdfStride",rb_eNetcdfError);
  rb_eNetcdfBadname = rb_define_class("NetcdfBadname",rb_eNetcdfError);
  /* N.B. following must match value in ncx.h */ 
  rb_eNetcdfRange = rb_define_class("NetcdfRange",rb_eNetcdfError);
  rb_eNetcdfNomem = rb_define_class("NetcdfNomem",rb_eNetcdfError);
  /* Global error status */
  rb_eNetcdfEntool = rb_define_class("NetcdfEntool",rb_eNetcdfError);
  rb_eNetcdfExdr = rb_define_class("NetcdfExdr",rb_eNetcdfError);
  rb_eNetcdfSyserr = rb_define_class("NetcdfSyserr",rb_eNetcdfError);
  /* Global options variable. Used to determine behavior of error handler. */
  rb_eNetcdfFatal = rb_define_class("NetcdfFatal",rb_eNetcdfError);

  /* Class Constants Definition */
  rb_define_const(cNetCDF, "NC_NOWRITE", INT2FIX(NC_NOWRITE));
  rb_define_const(cNetCDF, "NC_WRITE", INT2FIX(NC_WRITE));
  rb_define_const(cNetCDF, "NC_SHARE", INT2FIX(NC_SHARE));
  rb_define_const(cNetCDF, "NC_CLOBBER", INT2FIX(NC_CLOBBER));
  rb_define_const(cNetCDF, "NC_NOCLOBBER", INT2FIX(NC_NOCLOBBER));
#if NCVER >= 400
  rb_define_const(cNetCDF, "NC_64BIT_OFFSET", INT2FIX(NC_64BIT_OFFSET));
     /* NC_64BIT_OFFSET supports large files in the class data format */ 
  rb_define_const(cNetCDF, "NC_NETCDF4", INT2FIX(NC_NETCDF4));
  rb_define_const(cNetCDF, "NC_CLASSIC_MODEL", INT2FIX(NC_CLASSIC_MODEL));
     /* for use as ( NC_NETCDF4 | NC_CLASSIC_MODEL ) to ensure the classic 
        data model in NetCDF4 by disabling new features like groups */
  rb_define_const(cNetCDF, "NC_ENDIAN_NATIVE", INT2FIX(NC_ENDIAN_NATIVE));
  rb_define_const(cNetCDF, "NC_ENDIAN_LITTLE", INT2FIX(NC_ENDIAN_LITTLE));
  rb_define_const(cNetCDF, "NC_ENDIAN_BIG", INT2FIX(NC_ENDIAN_BIG));
#endif
 
#ifdef NARRAY_BIGMEM
  rb_define_const(cNetCDF, "SUPPORT_BIGMEM", Qtrue);
#else
  rb_define_const(cNetCDF, "SUPPORT_BIGMEM", Qfalse);
#endif

  /* Difinitions of the ruby methods */
  /* The methods of the NetCDF class */
  rb_define_singleton_method(cNetCDF,"libvers",NetCDF_inq_libvers,0);
  rb_define_singleton_method(cNetCDF,"nc_open",NetCDF_open,2);
  rb_define_method(cNetCDF,"clone",NetCDF_clone,0);
  rb_define_method(cNetCDF,"close",NetCDF_close,0);
  /* rb_define_singleton_method(cNetCDF,"new",NetCDF_open,2); */
  rb_define_singleton_method(cNetCDF,"nc_create",NetCDF_create,2);
  rb_define_method(cNetCDF,"def_dim",NetCDF_def_dim,2);
  rb_define_method(cNetCDF,"def_var",NetCDF_def_var,3);
  rb_define_method(cNetCDF,"put_attraw",NetCDF_put_att,3);
  rb_define_method(cNetCDF,"redef",NetCDF_redef,0);
  rb_define_method(cNetCDF,"enddef",NetCDF_enddef,0);
  rb_define_method(cNetCDF,"define_mode?",NetCDF_whether_in_define_mode,0);
  rb_define_method(cNetCDF,"fill",NetCDF_fill,1);
  rb_define_method(cNetCDF,"ndims",NetCDF_ndims,0);
  rb_define_method(cNetCDF,"nvars",NetCDF_nvars,0);
  rb_define_method(cNetCDF,"natts",NetCDF_natts,0);
  rb_define_method(cNetCDF,"sync",NetCDF_sync,0);
  rb_define_method(cNetCDF,"path",NetCDF_path,0);
  rb_define_method(cNetCDF,"dim",NetCDF_dim,1);
  rb_define_method(cNetCDF,"var",NetCDF_var,1);
  rb_define_method(cNetCDF,"att",NetCDF_att,1);
  rb_define_method(cNetCDF,"unlimited",NetCDF_unlimited,0);
  rb_define_private_method(cNetCDF,"id2var",NetCDF_id2var,1);
  rb_define_private_method(cNetCDF,"id2dim",NetCDF_id2dim,1);
  rb_define_private_method(cNetCDF,"id2att",NetCDF_id2att,1);
  rb_define_method(cNetCDF,"==",NetCDF_eql,1); 
  /* rb_define_method(cNetCDF,"eql?",NetCDF_eql,1); */

  /* The methods of the NetCDFDim class */
  rb_define_method(cNetCDFDim,"clone",NetCDF_dim_clone,0);
  rb_define_method(cNetCDFDim,"length",NetCDF_dim_length,0);
  rb_define_method(cNetCDFDim,"name=",NetCDF_dim_name,1);
  rb_define_method(cNetCDFDim,"name",NetCDF_dim_inqname,0);
  rb_define_method(cNetCDFDim,"unlimited?",NetCDF_dim_whether_unlimited,0);
  rb_define_method(cNetCDFDim,"==",NetCDF_dim_eql,1);
  /* rb_define_method(cNetCDFDim,"eql?",NetCDF_dim_eql,1); */

  /* The methods of the NetCDFAtt class */
  rb_define_method(cNetCDFAtt,"clone",NetCDF_att_clone,0);
  rb_define_method(cNetCDFAtt,"name",NetCDF_att_inq_name,0);
  rb_define_method(cNetCDFAtt,"name=",NetCDF_att_rename,1);
  rb_define_method(cNetCDFAtt,"delete",NetCDF_att_delete,0);
  rb_define_method(cNetCDFAtt,"copy",NetCDF_att_copy,1);
  rb_define_method(cNetCDFAtt,"atttype",NetCDF_att_atttype,0);
  rb_define_method(cNetCDFAtt,"typecode",NetCDF_att_typecode,0);
  rb_define_method(cNetCDFAtt,"==",NetCDF_att_eql,1);
  /* rb_define_method(cNetCDFAtt,"eql?",NetCDF_att_eql,1); */
  rb_define_method(cNetCDFAtt,"putraw",NetCDF_att_put,2);
  rb_define_method(cNetCDFAtt,"get",NetCDF_att_get,0);

  /* The methods of the NetCDFVar class */
#if NCVER >= 400
  rb_define_method(cNetCDFVar,"deflate",NetCDF_var_deflate,-1);
  rb_define_method(cNetCDFVar,"deflate_params",NetCDF_var_deflate_params,0);
  rb_define_method(cNetCDFVar,"endian=",NetCDF_var_set_endian,1);
  rb_define_method(cNetCDFVar,"endian",NetCDF_var_endian,0);
#endif
  rb_define_method(cNetCDFVar,"clone",NetCDF_var_clone,0);
  rb_define_method(cNetCDFVar,"name",NetCDF_var_inq_name,0);
  rb_define_method(cNetCDFVar,"ndims",NetCDF_var_ndims,0);
  rb_define_method(cNetCDFVar,"vartype",NetCDF_var_vartype,0);
  rb_define_method(cNetCDFVar,"typecode",NetCDF_var_typecode,0);
  rb_define_method(cNetCDFVar,"ntype",NetCDF_var_vartype,0);
  rb_define_method(cNetCDFVar,"natts",NetCDF_var_natts,0);
  rb_define_method(cNetCDFVar,"file",NetCDF_var_file,0);
  rb_define_method(cNetCDFVar,"name=",NetCDF_var_rename,1);
  rb_define_method(cNetCDFVar,"att",NetCDF_var_att,1);
  rb_define_method(cNetCDFVar,"put_attraw",NetCDF_put_att_var,3);
  rb_define_method(cNetCDFVar,"dims",NetCDF_var_dims,0);
  rb_define_method(cNetCDFVar,"dim",NetCDF_var_dim,1);
  /*rb_define_private_method(cNetCDFVar,"id2dim",NetCDF_var_id2dim,1); */
  rb_define_private_method(cNetCDFVar,"id2att",NetCDF_var_id2att,1);
  rb_define_method(cNetCDFVar,"==",NetCDF_var_eql,1);
  /* rb_define_method(cNetCDFVar,"eql?",NetCDF_var_eql,1); */

  /* The "get*" or "put*" methods in the NetCDFVar class */
  rb_define_method(cNetCDFVar,"put_var_char",NetCDF_put_var_char,1);
  rb_define_method(cNetCDFVar,"put_var_byte",NetCDF_put_var_byte,1);
  rb_define_method(cNetCDFVar,"put_var_sint",NetCDF_put_var_short,1);
  rb_define_method(cNetCDFVar,"put_var_int",NetCDF_put_var_int,1);
  rb_define_method(cNetCDFVar,"put_var_sfloat",NetCDF_put_var_float,1);
  rb_define_method(cNetCDFVar,"put_var_float",NetCDF_put_var_double,1);

  rb_define_method(cNetCDFVar,"put_vars_char",NetCDF_put_vars_char,4);
  rb_define_method(cNetCDFVar,"put_vars_byte",NetCDF_put_vars_byte,4);
  rb_define_method(cNetCDFVar,"put_vars_sint",NetCDF_put_vars_sint,4);
  rb_define_method(cNetCDFVar,"put_vars_int",NetCDF_put_vars_int,4);
  rb_define_method(cNetCDFVar,"put_vars_sfloat",NetCDF_put_vars_float,4);
  rb_define_method(cNetCDFVar,"put_vars_float",NetCDF_put_vars_double,4);

  rb_define_method(cNetCDFVar,"put_var1_char",NetCDF_put_var1_char,2);
  rb_define_method(cNetCDFVar,"put_var1_byte",NetCDF_put_var1_byte,2);
  rb_define_method(cNetCDFVar,"put_var1_sint",NetCDF_put_var1_sint,2);
  rb_define_method(cNetCDFVar,"put_var1_int",NetCDF_put_var1_int,2);
  rb_define_method(cNetCDFVar,"put_var1_sfloat",NetCDF_put_var1_float,2);
  rb_define_method(cNetCDFVar,"put_var1_float",NetCDF_put_var1_double,2);

  rb_define_method(cNetCDFVar,"get_var_char",NetCDF_get_var_char,0);
  rb_define_method(cNetCDFVar,"get_var_byte",NetCDF_get_var_byte,0);
  rb_define_method(cNetCDFVar,"get_var_sint",NetCDF_get_var_sint,0);
  rb_define_method(cNetCDFVar,"get_var_int",NetCDF_get_var_int,0);
  rb_define_method(cNetCDFVar,"get_var_sfloat",NetCDF_get_var_float,0);
  rb_define_method(cNetCDFVar,"get_var_float",NetCDF_get_var_double,0);
  
  rb_define_method(cNetCDFVar,"get_vars_char",NetCDF_get_vars_char,3);
  rb_define_method(cNetCDFVar,"get_vars_byte",NetCDF_get_vars_byte,3);
  rb_define_method(cNetCDFVar,"get_vars_sint",NetCDF_get_vars_sint,3);
  rb_define_method(cNetCDFVar,"get_vars_int",NetCDF_get_vars_int,3);
  rb_define_method(cNetCDFVar,"get_vars_sfloat",NetCDF_get_vars_float,3);
  rb_define_method(cNetCDFVar,"get_vars_float",NetCDF_get_vars_double,3);

  rb_define_method(cNetCDFVar,"get_var1_char",NetCDF_get_var1_char,1);
  rb_define_method(cNetCDFVar,"get_var1_byte",NetCDF_get_var1_byte,1);
  rb_define_method(cNetCDFVar,"get_var1_sint",NetCDF_get_var1_sint,1);
  rb_define_method(cNetCDFVar,"get_var1_int",NetCDF_get_var1_int,1);
  rb_define_method(cNetCDFVar,"get_var1_sfloat",NetCDF_get_var1_float,1);
  rb_define_method(cNetCDFVar,"get_var1_float",NetCDF_get_var1_double,1);
}
