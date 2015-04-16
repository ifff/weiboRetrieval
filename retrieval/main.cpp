#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <functional>
#include <algorithm> 
#include "Index.hpp"
#include "IndexManager.hpp"
#include "BasicDocStream.hpp"
#include "RetMethodManager.hpp"
#include "ResultFile.hpp"
#include "IndexedReal.hpp"
#include "SimpleKLRetMethod.hpp"

#include "include/MyDocModel.hpp"
#include "include/MyRetMethod.hpp"
#include "include/Cmdline.h"

using namespace lemur::api;
using namespace lemur::retrieval;
using namespace std;


int main(int argc, char** argv) 
{
    Cmdline cmd(argc, argv);
    try {
        cmd.check_param();
    }
    catch(string &ex) {
        cerr << ex << endl;
        return 0;
    }

    // Get parameters from cmdline
    const string query_path = cmd.get_value("query", "");
    const string expand_path = cmd.get_value("expand", "");
    const string result_path = cmd.get_value("output", "");
    const string index_path = cmd.get_value("index", "");
    const string ssf_path = cmd.get_value("ssf", "");
    const int year = cmd.get_value("year", 2013);

    const int query_count = cmd.get_value("count", 60);
    // query update parameter
    const float alpha = cmd.get_value("alpha",0.2);
    const float beta = cmd.get_value("beta", 0.2);
    // second stage parameter
    const int docNum = cmd.get_value("doc", 0);
    const int termNum = cmd.get_value("term", 0); 
    
    //Multi Run Parameter
    const int total = cmd.get_value("total", 1);
    const int remain = cmd.get_value("remain", 0);


    string resultFormat = "trec";
    string retModel = "kl";
    // Ret parameters
    bool ignoreWeights = true;  
    bool doingRelModel = false;
    int fbDocCount = docNum;
    int resultCount = 5000;
    // Query parameter
    static SimpleKLParameter::QueryModelParam qryPrm;
    qryPrm.adjScoreMethod = SimpleKLParameter::adjustedScoreMethods(1); 
    qryPrm.qryNoise = 0;
    qryPrm.fbMethod = SimpleKLParameter::QueryUpdateMethod(0);  //Mixture
    qryPrm.fbCoeff = beta; 
    qryPrm.fbTermCount = termNum;
    qryPrm.fbPrTh = 0.001;
    qryPrm.fbPrSumTh = 1;
    qryPrm.fbMixtureNoise = 0.6; 
    qryPrm.emIterations = 50;

    // Document parameter
    static SimpleKLParameter::DocSmoothParam docPrm;
    docPrm.smthMethod = SimpleKLParameter::SmoothMethod(1);     //Dirichlet
    docPrm.smthStrategy = SimpleKLParameter::SmoothStrategy(1); //Interpolate
    docPrm.DirPrior = 100;    // mu=100

    for(int qid=1; qid<=query_count; qid++) {
        if(qid % total != remain) continue;
        cout << "process qid " << qid << " with alpha=" << alpha << ", beta=" << beta << endl;
        cout << "doc: " << docNum << ", term: " << termNum << endl;
        // basic parameters
        ostringstream ossIndexPath, ossQueryPath, ossResultPath, ossSupportPath,ossFreebasePath,ossExQueryPath;
        if(year == 2013) {
            ossIndexPath <<  index_path << "/docEx/docEx" << qid << "/base.key";
            ossSupportPath << ssf_path << "/docEx/" << qid << ".supp";
        } else {
            ossIndexPath <<  index_path << "/DocEx/docEx" << qid << "/basic.key";
            ossSupportPath << ssf_path << "/DocExSSF/" << qid << ".supp";
        }
        ossQueryPath <<  query_path << "/" << qid << ".query";
        ossFreebasePath <<  expand_path << "/" << qid << ".query";
        ossExQueryPath << "shortQuery2012/" << qid << ".query";
        ossResultPath << result_path << "/"  << qid << ".res";

        string indexPath = ossIndexPath.str(); 
        string supportPath = ossSupportPath.str();
        string queryPath = ossQueryPath.str();
        string freebasePath = ossFreebasePath.str();
        string resultPath = ossResultPath.str();

        // open the index
        Index *ind;
        try {
            ind = IndexManager::openIndex(indexPath.c_str());
        }
        catch (Exception &ex) {
            ex.writeMessage();
            throw Exception("Can't open index");
        }


        // load the query
        DocStream *qryStream, *fbQryStream, *exQryStream;
        try {
            qryStream = new lemur::parse::BasicDocStream(queryPath.c_str());
            //fbQryStream = new lemur::parse::BasicDocStream(freebasePath.c_str());
            //exQryStream = new lemur::parse::BasicDocStream(ossExQueryPath.str().c_str());
        }
        catch(Exception &ex) {
            ex.writeMessage();
            throw Exception("Can't open query file");
        }

        // open result file
        ofstream result(resultPath.c_str());
        ResultFile resFile(resultFormat.c_str());
        resFile.openForWrite(result, *ind);

        // build the model
        ArrayAccumulator accumulator(ind->docCount());
        IndexedRealVector results(ind->docCount());
        MyRetMethod *model = NULL;
        model = new MyRetMethod(*ind, supportPath, accumulator);
        // set model parameters
        model->setQueryModelParam(qryPrm);
        model->setDocSmoothParam(docPrm);

        qryStream->startDocIteration();
        //fbQryStream->startDocIteration();
        //exQryStream->startDocIteration();
        TextQuery *q, *eq;

        if (qryStream->hasMore()) {
            Document *d = qryStream->nextDoc();
            q = new TextQuery(*d);
            cout << "query : "<< q->id() << endl;

            const TermQuery *tq = q;
            MyQueryModel *qm = new MyQueryModel(*tq, *ind);
            cerr << "query count " << qm->totalCount() << endl;
            
            // load the expand query
            /*if(fbQryStream->hasMore()) {
                Document *ed = fbQryStream->nextDoc();
                eq = new TextQuery(*ed);
                const TermQuery *etq = eq;
                lemur::retrieval::MyQueryModel *eqm = new lemur::retrieval::MyQueryModel(*etq, *ind);
                
                // load the expand query and update the original query
                if(exQryStream->hasMore()) {
                    Document *edshort = exQryStream->nextDoc();
                    TextQuery *eqshort = new TextQuery(*edshort);
                    const TermQuery *etqshort = eqshort;
                    lemur::retrieval::MyQueryModel *eqmshort = new lemur::retrieval::MyQueryModel(*etqshort, *ind);
                    cerr << "expand query count " << eqmshort->totalCount() << endl;
                    if(eqmshort->totalCount() > 0) {
                        model->expandQuery(*eqm, *eqmshort, 0.5);   
                    }
                    delete eqmshort;
                }


                cerr << "freebase query count " << eqm->totalCount() << endl;
                model->expandQuery(*qm, *eqm, 1.0-alpha);          
                delete eqm;
            }  */

            model->scoreCollection(*qm, results);
            results.Sort();
             
            // prune to number of feedback docs.
            // second stage model-based PRF
            if (fbDocCount > 0 && results.size() > fbDocCount) {
                results.erase(results.begin() + fbDocCount, results.end());
            }
            if (fbDocCount > 0) {
                PseudoFBDocs *topDoc = new PseudoFBDocs(results, fbDocCount, ignoreWeights);
                model->updateTextQuery(*qm, *topDoc);
                model->scoreCollection(*qm, results);
                results.Sort();
                delete topDoc;
            } 
              
            cout << "write result to " << resultPath << endl;
            resFile.writeResults(q->id(), &results, resultCount);
            delete q;
            delete eq;
            delete qm;
        }

        // close result file
        result.close();
        delete model;
        delete qryStream;
        delete fbQryStream;
        delete ind;
    }
    return 0;
}
