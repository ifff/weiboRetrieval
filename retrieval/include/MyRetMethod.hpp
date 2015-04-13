#ifndef _MYRETMETHOD_HPP
#define _MYRETMETHOD_HPP

#include "SimpleKLRetMethod.hpp"
using namespace lemur::api;


namespace lemur {
    namespace retrieval {
        class MyQueryModel : public SimpleKLQueryModel{
            public:
                MyQueryModel(const lemur::api::TermQuery &qry, const lemur::api::Index &dbIndex) : 
                    SimpleKLQueryModel(qry, dbIndex) {}

                MyQueryModel(const lemur::api::Index &dbIndex) :
                    SimpleKLQueryModel(dbIndex) {}

                void debug()
                {
                    cout << "---- query ----" << endl;
                    startIteration();
                    while (hasMore()) {
                        lemur::api::QueryTerm *qt = nextTerm();
                        cerr << qt->id() << ":" << ind.term(qt->id()) << ":" << qt->weight() << endl;
                        delete qt;
                    }  
                }
                                                 
                bool include(int termId)
                {
                    startIteration();
                    while (hasMore()) {
                        lemur::api::QueryTerm *qt = nextTerm();
                        if(qt->id() == termId) {
                            delete qt;
                            return true;
                        }
                        delete qt;
                    }  
                    return false;
                }
                                                
				
                void expandQueryByMap(std::map<int,double>& fb, double origModCoeff) {
                    if (!qm) {
                        qm = new lemur::api::IndexedRealVector();
                    } else {
                        qm->clear();
                    }
                   
                    cout << "stage 1" << endl;
                    debug();

					std::map<int,double>::iterator it;
					for(it=fb.begin();it!=fb.end();++it)
					{
						int termID = it->first;
                        IndexedReal entry;
 
                        if(!include(termID))   {
                            entry.ind = termID;
                            entry.val = 0;
                            qm->push_back(entry);
                        }
					}    
                    
                    cout << "stage 2" << endl;
                    debug();

                    // get original query term count
                    double countSum = totalCount();
                    startIteration();
                    while (hasMore()) {
                        lemur::api::QueryTerm *qt = nextTerm();
                        setCount(qt->id(), qt->weight()*origModCoeff/countSum);
                        delete qt;
                    }    
                    
                    cout << "stage 3" << endl;
                    debug();


					for(it=fb.begin();it!=fb.end();++it)
					{
						int termID = it->first;
						double score = it->second;
						incCount(termID, score*(1.0-origModCoeff));
					}
                    
                    cout << "stage 4" << endl;
                    debug();

                    //Sum w in Q qtf * log(qtcf/termcount);
                    colQLikelihood = 0;
                    colQueryLikelihood();
                    colKLComputed = false;     
				}

                void expandQuery(MyQueryModel &exRep, double origModCoeff) {
                    if (!qm) {
                        qm = new lemur::api::IndexedRealVector();
                    } else {
                        qm->clear();
                    }

                    // get original query term count
                    double countSum = totalCount();

                    // add expand query's term, init weight as zero
                    exRep.startIteration();
                    while (exRep.hasMore()) {
                        lemur::api::QueryTerm *qt = exRep.nextTerm();
                        IndexedReal entry;
                        if(!include(qt->id()))   {
                            entry.ind = qt->id();
                            //entry.val = qt->weight();
                            entry.val = 0;
                            qm->push_back(entry);
                        }
                        delete qt;
                    }
                    qm->Sort();
                    // cout << "after push words" << endl;
                    // cout << "original total count = " << totalCount() << endl;
                    // debug();

                    // discounting the original model
                    startIteration();
                    while (hasMore()) {
                        lemur::api::QueryTerm *qt = nextTerm();
                        setCount(qt->id(), qt->weight()*origModCoeff/countSum);
                        delete qt;
                    }    
                    // debug();

                    // now adding the new model
                    double exCountSum = exRep.totalCount();
                    exRep.startIteration();
                    while (exRep.hasMore()) {
                        lemur::api::QueryTerm *qt = exRep.nextTerm();
                        incCount(qt->id(), qt->weight()*(1.0-origModCoeff)/exCountSum);
                        delete qt;
                    }
                    debug();

                    //Sum w in Q qtf * log(qtcf/termcount);
                    colQLikelihood = 0;
                    colQueryLikelihood();
                    colKLComputed = false;      
                }


                void interpolateWith(const lemur::langmod::UnigramLM &qModel,
                            double origModCoeff,
                            int howManyWord,
                            double prSumThresh,
                            double prThresh) {
                    if (!qm) {
                        qm = new lemur::api::IndexedRealVector();
                    } else {
                        qm->clear();
                    }

                    qModel.startIteration();
                    while (qModel.hasMore()) {
                        IndexedReal entry;
                        qModel.nextWordProb((TERMID_T &)entry.ind,entry.val);
                        qm->push_back(entry);

                    }
                    qm->Sort();

                    double countSum = totalCount();


                    startIteration();
                    while (hasMore()) {
                        QueryTerm *qt = nextTerm();
                        setCount(qt->id(), qt->weight()*origModCoeff/countSum);
                        delete qt;
                    }

                    cout << "-------- FB terms --------" << endl;
                    double prSum = 0;
                    int wdCount = 0;
                    IndexedRealVector::iterator it;
                    it = qm->begin();
                    while (it != qm->end() && prSum < prSumThresh &&
                                wdCount < howManyWord && (*it).val >=prThresh) {
                        incCount((*it).ind, (*it).val*(1-origModCoeff));
                        cout << ind.term(it->ind) << endl;
                        prSum += (*it).val;
                        it++;
                        wdCount++;
                    }
                    cout << "--------------------------" << endl;


                    colQLikelihood = 0;
                    colQueryLikelihood();
                    colKLComputed = false;
                }
        };



        class MyScoreFunc : public SimpleKLScoreFunc {

            double matchedTermWeight(const lemur::api::QueryTerm *qTerm, 
                        const lemur::api::TextQueryRep *qRep, 
                        const lemur::api::DocInfo *info,
                        const lemur::api::DocumentRep *dRep) const { 
                double w = qTerm->weight();
                double d = dRep->termWeight(qTerm->id(),info);
                double l = log(d);
                double score = w*l;
                return score;
            }

            protected:
            MyScoreFunc *scFunc; 

        };

        class MyRetMethod : public SimpleKLRetMethod {
            public:
                MyRetMethod(const lemur::api::Index &dbIndex, 
                            const string &supportFileName, 
                            lemur::api::ScoreAccumulator &accumulator): SimpleKLRetMethod(dbIndex, supportFileName, accumulator) {
                    scFunc = new MyScoreFunc();
                }

                void expandQueryByMap(MyQueryModel &origRep, std::map<int, double>&fb, double orgCoeff) {
                    origRep.expandQueryByMap(fb, orgCoeff);
                }

                void expandQuery(MyQueryModel &origRep, MyQueryModel &exRep, double orgCoeff) {
                    origRep.expandQuery(exRep, orgCoeff);
                }

        };
    }
}

#endif /* _MYRETMETHOD_HPP */
