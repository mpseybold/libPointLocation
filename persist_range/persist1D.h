
#include "ptree.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>


namespace PERSIST1D {

    static gint
    cmp (gconstpointer k1, gconstpointer k2) {
    gint key1 = GPOINTER_TO_INT(k1);
    gint key2 = GPOINTER_TO_INT(k2);
    glong r = key1 - key2;
    return r < 0 ? -1 : (r == 0 ? 0 : 1);
    }

    struct ID_Val {
        int id;
        double val;

        ID_Val(int _id, int _val) :
         id(_id), val(_val) {};
    };

    bool cmp_id_val(ID_Val& a, ID_Val& b) {
        int val = a.val - b.val;

        return val < 0;
    }

    int cmp_tree_keys() {

    }



    class Persist1D {
        public:

            PTree *tree;

            int seed  = 1337;
            int n;
            int width;

            int* idx;

            int *y, *xl, *xr;

            gint max_version;

            Persist1D() {}

            Persist1D(std::vector<std::vector<double>> segments) {
                n = segments.size();

                y = new int[n];
                xl = new int[n];
                xr = new int[n];
                idx = new int[2*n];

                auto id_val = std::vector<ID_Val>();

                for (int i = 0; i < n; ++i) {
                    xl[i] = (int)segments[i][0];
                    xr[i] = (int)segments[i][2];
                    y[i] = (int)segments[i][1];
                    idx[2*i] = 2*i;
                    idx[2*i+1] = 2*i+1;
                }

                for (int i = 0; i < n; ++i) {
                        id_val.push_back(ID_Val(2*i, xl[i]));
                        id_val.push_back(ID_Val(2*i + 1, xr[i]));
                }

                std::sort(id_val.begin(), id_val.end(), cmp_id_val);

                for (int i = 0; i <  id_val.size(); ++i) {
                    idx[i] = id_val[i].id;
                }

                tree = p_tree_new (cmp);

                for (int i = 0; i < 2*n; ++i) {
                    gint key = y[ idx[i]/2 ];
                    int index = idx[i];
                    if( idx[i] % 2 ){
                    p_tree_remove (tree, GINT_TO_POINTER( key ) );
                    p_tree_next_version (tree);
                    } else {
                    p_tree_insert (tree, GINT_TO_POINTER( key      ),
                                        GINT_TO_POINTER( idx[i]/2 ) );
                    p_tree_next_version (tree);
                    }
                }

                max_version = p_tree_current_version(tree);
            }

            int v_query(int y1, int y2, int x) {
                MPS_COUNTER = 0;
                gint q_version = findVersionForX( x );

                if (y2 < y1) {
                    int aux = y2;
                    y2 = y1;
                    y1 = aux;
                }

                gint key_low = y1;
                gint key_high = y2;


                gint result = p_tree_rangesearch_v(tree, q_version, 
                GINT_TO_POINTER(key_low), GINT_TO_POINTER(key_high));

                return result;
            }

            guint findVersionForX( int x ){
                MPS_COUNTER++;
                //  Binary search for the root where range search starts
                guint l=0;
                guint r=2*n-1;
                    while(l < r){
                        int m = (l + r) / 2;
                        int xm = (idx[m] % 2)? xr[idx[m]/2] : xl[ idx[m]/2 ];
                        if(xm < x)
                            l = m + 1;
                        else
                            r = m;
                    }
                    return l;
            }


        
    };

}
