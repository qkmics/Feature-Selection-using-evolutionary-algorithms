#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

class Individual  //个体
{
private:
    vector<bool> genes;

public:
    Individual(void)
    {

    }
    Individual(int length)
    {
        srand((unsigned)rand()+(unsigned)(time(0)));      //注意此处的随机数产生方式
        for(int i=0;i<length;i++)
            genes.push_back((rand() % 2));
    }
    void setGenes(vector<bool> genes)
    {
        this->genes=genes;
    }
    vector<bool> getGenes(void)
    {
        return this->genes;
    }
    int clacuFitness(void)
    {
        int fitness=0;
        for(bool gene:this->genes)
            if(gene==1)
                fitness++;
        return fitness;
    }

    Individual& operator=(const Individual& indiv)
    {
        this->genes=indiv.genes;
        return *this;
    }

    void mutation(void)
    {
        srand((unsigned)rand()+(unsigned)(time(0)));
        int point=rand()%genes.size();
        //cout<<"mutation point: "<<point<<endl;
        genes[point]=1-genes[point];
    }

    void print(void)
    {
        for(bool gene:genes)
            cout<<gene;
        cout<<"fitness: "<<clacuFitness()<<endl;
    }
};

void individualCrossover(Individual parent1,Individual parent2,Individual& child1,Individual& child2)
{
    srand((unsigned)rand()+(unsigned)(time(0)));

    vector<bool> parent1Genes,parent2Genes,child1Genes,child2Genes;
    parent1Genes=parent1.getGenes();
    parent2Genes=parent2.getGenes();
    int length=parent1Genes.size();
    int point=(rand()%length);
    //cout<<endl<<"point="<<point<<endl;
    for(int i=0;i<point;i++)
    {
       child1Genes.push_back(parent1Genes[i]);
       child2Genes.push_back(parent2Genes[i]);
    }

    for(int i=point;i<length;i++)
    {
       child1Genes.push_back(parent2Genes[i]);
       child2Genes.push_back(parent1Genes[i]);
    }

    child1.setGenes(child1Genes);
    child2.setGenes(child2Genes);

    //cout<<"child1.print();"<<endl;
    //child1.print();
    //cout<<"child2.print();"<<endl;
    //child2.print();

}
class Population //种群
{
private:
    vector<Individual> individuals;
    void sort(void) //按照适应度从大到小
    {
        Individual temp;
        int popSize=individuals.size();
        for(int i=0;i<popSize;i++)
            for(int j=0;j<popSize;j++)
            {
                if(individuals[i].clacuFitness()>individuals[j].clacuFitness())
                {
                    temp=individuals[i];
                    individuals[i]=individuals[j];
                    individuals[j]=temp;
                }
            }
    }
    void select(int num)  //选择，保留前num个个体
    {
        individuals.erase(individuals.begin()+num-1,individuals.end());
    }

    void crossover() //交配
    {
        //cout<<"crossover"<<endl;
        int parentNum=individuals.size()-2;
        for(int i=0;i<parentNum;i+=2)
        {
            Individual child1,child2;
            individualCrossover(individuals[i],individuals[i+1],child1,child2);
            individuals.push_back(child1);
            individuals.push_back(child2);
        }

    }
    bool fitnessGet(int targetFitness)
    {
        if(this->individuals[0].clacuFitness()>=targetFitness)
            return true;
        else
            return false;
    }
    void mutation(void)
    {
        for(int i=0;i<individuals.size();i++)
        {
            individuals[i].mutation();
        }
    }
    void print(void)
    {
        for(Individual indiv:individuals)
            indiv.print();
        cout<<endl;
    }
public:
    Population(int populationSize,int length)  //种群大小，个体的基因长度
    {
        for(int i=0;i<populationSize;i++)
        {
            Individual individual(length);
            this->individuals.push_back(individual);
        }
    }

    void selectCrossover(int num)   //选择num数量的个体，交配
    {
        mutation();  //变异
        sort();  //按适应度从大到小排序
        select(num);  //选择
        crossover();  //交配
        sort();  //按适应度从大到小排序
        //print();
    }

    Individual process(int targetFitness,int num,int maxGeneration)
    {
        int generation=0;
        selectCrossover(num);
        while(fitnessGet(targetFitness)==false&&maxGeneration--)
        {
            if(generation%10==0)
            {
                cout<<"generation:"<<generation<<endl;
                cout<<"Greatest fitness:"<<individuals[0].clacuFitness()<<endl;
                individuals[0].print();
            }

            generation++;
            selectCrossover(num);
        }
        return individuals[0];
    }

};
int main()
{
    int populationSize,geneLength,targetFitness,selectNum,maxGeneration;
    populationSize=20;
    geneLength=150;
    targetFitness=150;
    selectNum=20;   //偶数
    maxGeneration=1000;

    Population population(populationSize,geneLength);
    Individual targetIndiv=population.process(targetFitness,selectNum,maxGeneration);
    return 0;
}
