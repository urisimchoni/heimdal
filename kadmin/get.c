/*
 * Copyright (c) 1997 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. All advertising materials mentioning features or use of this software 
 *    must display the following acknowledgement: 
 *      This product includes software developed by Kungliga Tekniska 
 *      H�gskolan and its contributors. 
 *
 * 4. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */

#include "kadmin_locl.h"
#include <parse_units.h>

RCSID("$Id$");

static void
print_entry(kadm5_principal_ent_t princ)
{
    char buf[1024];
    
    krb5_unparse_name_fixed(context, princ->principal, buf, sizeof(buf));
    printf("%24s: %s\n", "Principal", buf);
    timeval2str(princ->princ_expire_time, buf, sizeof(buf));
    printf("%24s: %s\n", "Principal expires", buf);
	    
    timeval2str(princ->pw_expiration, buf, sizeof(buf));
    printf("%24s: %s\n", "Password expires", buf);
	    
    timeval2str(princ->last_pwd_change, buf, sizeof(buf));
    printf("%24s: %s\n", "Last password change", buf);
	
    deltat2str(princ->max_life, buf, sizeof(buf));
    printf("%24s: %s\n", "Max ticket life", buf);

    deltat2str(princ->max_renewable_life, buf, sizeof(buf));
    printf("%24s: %s\n", "Max renewable life", buf);
    printf("%24s: %d\n", "Kvno", princ->kvno);
    printf("%24s: %d\n", "Mkvno", princ->mkvno);
    printf("%24s: %s\n", "Policy", princ->policy ? princ->policy : "none");
    timeval2str(princ->last_success, buf, sizeof(buf));
    printf("%24s: %s\n", "Last successful login", buf);
    timeval2str(princ->last_failed, buf, sizeof(buf));
    printf("%24s: %s\n", "Last failed login", buf);
    printf("%24s: %d\n", "Failed login count", princ->fail_auth_count);
    timeval2str(princ->mod_date, buf, sizeof(buf));
    printf("%24s: %s\n", "Last modified", buf);
    krb5_unparse_name_fixed(context, princ->mod_name, buf, sizeof(buf));
    printf("%24s: %s\n", "Modifier", buf);
    attr2str (princ->attributes, buf, sizeof(buf));
    printf("%24s: %s\n", "Attributes", buf);
    printf("\n");
}

int
get_entry(int argc, char **argv)
{
    kadm5_principal_ent_rec princ;
    krb5_error_code ret;
    krb5_principal princ_ent;
    int i;

    for(i = 1; i < argc; i++){
	memset(&princ, 0, sizeof(princ));
	ret = krb5_parse_name(context, argv[i], &princ_ent);
	if(ret){
	    krb5_warn(context, ret, "krb5_parse_name(%s)", argv[i]);
	    continue;
	}
	ret = kadm5_get_principal(kadm_handle, princ_ent, 
				  &princ, KADM5_PRINCIPAL_NORMAL_MASK);
	krb5_free_principal(context, princ_ent);
	if(ret)
	    krb5_warn(context, ret, "%s", argv[i]);
	else {
	    print_entry(&princ);
	    kadm5_free_principal_ent(kadm_handle, &princ);
	}
    }
    return 0;
}

