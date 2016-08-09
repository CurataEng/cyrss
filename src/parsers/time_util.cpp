#include <cstring>
#include <cstdlib>

#include "rss/parsers/time_util.h"


namespace curata { namespace rss { namespace parsers { namespace time_util {


// stolen from newsbeuter
std::string w3cdtf_to_rfc822(const std::string& w3cdtf) {
    struct tm stm;
    memset(&stm, 0, sizeof (stm));
    stm.tm_mday = 1;

    char * ptr = strptime(w3cdtf.c_str(), "%Y", &stm);

    if (ptr != NULL) {
        ptr = strptime(ptr, "-%m", &stm);
    } else {
        return "";
    }

    if (ptr != NULL) {
        ptr = strptime(ptr, "-%d", &stm);
    }
    if (ptr != NULL) {
        ptr = strptime(ptr, "T%H", &stm);
    }
    if (ptr != NULL) {
        ptr = strptime(ptr, ":%M", &stm);
    }
    if (ptr != NULL) {
        ptr = strptime(ptr, ":%S", &stm);
    }

    int offs = 0;
    if (ptr != NULL) {
        if (ptr[0] == '+' || ptr[0] == '-') {
            unsigned int hour, min;
            if (sscanf(ptr+1,"%02u:%02u", &hour, &min)==2) {
                offs = 60*60*hour + 60*min;
                if (ptr[0] == '+')
                    offs = -offs;
                stm.tm_gmtoff = offs;
            }
        } else if (ptr[0] == 'Z') {
            stm.tm_gmtoff = 0;
        }
    }

    time_t t = mktime(&stm);
    time_t x = time(NULL);
    t += localtime(&x)->tm_gmtoff + offs;
    char datebuf[256];
    strftime (datebuf, sizeof (datebuf), "%a, %d %b %Y %H:%M:%S %z", gmtime(&t));
    return datebuf;
}

}}}} // curata::rss::parsers::time_util
