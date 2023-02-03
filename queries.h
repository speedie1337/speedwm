/* Queries
 *
 * Queries allow you to return the output of almost any function using the '-q' argument.
 */
static Query queries[] = {
    /* queries              functions              */
    { "get_tags",           get_tags               },
    { "get_used_tags",      get_used_tags          },
    { "get_numtags",        get_numtags            },
    { "get_layouts",        get_layouts            },
    { "get_numlayouts",     get_numlayouts         },
    { "get_barpos",         get_barpos             },
    { "get_bordersize",     get_bordersize         },
    { "get_hidebar",        get_hidebar            },
    { "get_snap",           get_snap               },
    { "get_queries",        get_queries            },
    { "get_font",           get_font               },
    { "get_mfact",          get_mfact              },
    { "get_mastercount",    get_mastercount        },
    { "get_resizehints",    get_resizehints        },
    { "get_lockfullscreen", get_lockfullscreen     },
    { "get_barpadding_ov",  get_barpadding_ov      },
    { "get_barpadding_oh",  get_barpadding_oh      },
    { "get_barpadding_iv",  get_barpadding_iv      },
    { "get_barpadding_ih",  get_barpadding_ih      },
};

