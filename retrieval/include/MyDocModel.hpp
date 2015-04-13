#ifndef _MYDOCMODEL_HPP
#define _MYDOCMODEL_HPP

#include "SimpleKLDocModel.hpp"

namespace lemur
{
    namespace retrieval
    {
        class MyDocModel : public SimpleKLDocModel {
            public:
                MyDocModel(lemur::api::DOCID_T docID, const lemur::langmod::UnigramLM &collectLM, 
                       int dl = 1, 
                       const double *prMass = NULL,
                       SimpleKLParameter::SmoothStrategy strat = SimpleKLParameter::INTERPOLATE) : 
                SimpleKLDocModel(docID, collectLM, dl, prMass, strat) {};
        };
    }
}


#endif /* _MYDOCMODEL_HPP */
