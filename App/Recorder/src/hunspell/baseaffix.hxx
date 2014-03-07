/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef _BASEAFF_HXX_
#define _BASEAFF_HXX_

class AffEntry
{
public:

protected:
    char *         appnd;
    char *         strip;
    unsigned char  appndl;
    unsigned char  stripl;
    char           numconds;
    char           opts;
    unsigned short aflag;
    union {
        char       conds[MAXCONDLEN];
        struct {
            char   conds1[MAXCONDLEN_1];
            char * conds2;
        } l;
    } c;
    char *           morphcode;
    unsigned short * contclass;
    short            contclasslen;
};

#endif
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/