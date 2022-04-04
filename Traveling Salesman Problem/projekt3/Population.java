import java.util.ArrayList;

public class Population {

    private ArrayList population;
    
    public Population(double omega) {
        population = new ArrayList();
    }

    public void add(Individual individual) {
        population.add(individual);

    }

    public boolean contains(Individual individual) {
        return population.contains(individual);
    }

    public void remove(Individual individual) {
        
        while(!(population.isEmpty())) {
            if(population.contains(individual)) {
                population.remove(individual);
            } 
            
        }
    }

    public int size() {
        return population.size();
    }

    public void epidemic() {


        
    }






    

    public city [] bestPath(){
    	return bestPath;
    }
}

