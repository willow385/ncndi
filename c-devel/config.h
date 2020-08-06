#ifndef CONFIG_H
#define CONFIG_H

#ifndef MPL_DEBUG_

// Change this to 1 for debugging messages
#define MPL_DEBUG_ 0

#endif

#if MPL_DEBUG_
    #define MPL_DEBUG(x) x;
#else
    #define MPL_DEBUG(x) ;
#endif

#endif /* CONFIG_H */
