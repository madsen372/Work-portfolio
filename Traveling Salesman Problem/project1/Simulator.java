import java.util.Scanner;
import java.lang.Math;

public class Simulator {
	private static final int SILENT = 1;
	private static final int VERBOSE = 2;
	private static final int EVENTS = 3;
	private static final int TIME = 4;
	
	
	private static int currentPopulation;
	private static int startPopulation;
	private static int maxPopulation;
	private static int mutationsInterval;	
	private static int deathInterval;
	private static int reproductionInterval;
	private static double comfortNormalization;
	private static double  simulationTime;
	

	public static void main(String[] args) {
		Scanner keyboard =  new Scanner(System.in);
		currentPopulation = 0;

		System.out.println("Indskriv startvaerdien af befolkningen");
		startPopulation = keyboard.nextInt();

		System.out.println("Indskriv det maksmimale antal af belfolkningen");
		maxPopulation = keyboard.nextInt();

		System.out.println("Indskriv comfort normalization term");
		comfortNormalization = keyboard.nextDouble();

		System.out.println("Indskriv doedsintervallet");
		deathInterval = keyboard.nextInt();

		System.out.println("Indskriv mutationsintervallet");
		mutationsInterval = keyboard.nextInt();

		System.out.println("Indskriv reproduktionsintervallet");
		reproductionInterval = keyboard.nextInt();
		
		System.out.println("Tast vaerdien af simulationtid");
		simulationTime = keyboard.nextDouble();
		
		System.out.println("Loggin type (1 = silent, 2 = verbose, 3 = events, 4 = time.)");
		int loggingType = keyboard.nextInt();
			if (TIME || EVENTS){
				int reportInterval = keyboard.nextInt();
			}

		
		City[] cities = CityGenerator.generate();

		Individual[] individuals = new Individual[startPopulation];
		
		Population pop = new Population(comfortNormalization);

		int eventCount = 0;
		
		EventQueue queue = new EventQueue();
		for(int i = 0; i<startPopulation; i++ ){
			individuals[i] = new Individual(cities);
			pop.add(individuals[i]);
			double fitness = pop.fitness(individuals[i]);
			
			queue.add(new Event(Event.REPRODUCTION, RandomUtils.getRandomTime(calculateReproduction(pop.fitness(individuals[i]))), individuals[i]));
			queue.add(new Event(Event.MUTATION, RandomUtils.getRandomTime(calculateMutation(pop.fitness(individuals[i]))), individuals[i]));
			queue.add(new Event(Event.DEATH, RandomUtils.getRandomTime(calculateDeath(pop.fitness(individuals[i]))), individuals[i]));

			if (loggingType == VERBOSE) {
				System.out.println("Simulating: " + queue.next());

			}




		}
		double simtime = 0;
		double timeCount = 0;
		while(queue.hasNext()){
			
			//System.out.println("Population Size: " + pop.size());
			Event event = queue.next();
			Individual individual = event.individual();
			char type = event.type();
			simtime +=event.time();
			
			if (Math.floor(simtime)>timeCount){
				timeCount=Math.floor(simtime)+1;
				//System.out.println(timeCount);
			}
			if (simtime>simulationTime){
				//System.out.println(pop.size());
				//System.out.println(simtime + " simulation time : " + simulationTime);
				break;
			}
			if (type == Event.DEATH){
				eventCount = eventCount+1;
				if (RandomUtils.getRandomValue(100) < probabilityOfDeath(pop.fitness(individual))*100){
					//System.out.println("han er ikke mere levende");
					pop.remove(individual);
				}
				else {
					queue.add(new Event(Event.DEATH, RandomUtils.getRandomTime(calculateDeath(pop.fitness(individual))), individual));
					//System.out.println("han er levende");
				}
			}
			else if (type == Event.MUTATION){
				eventCount = eventCount+1;
				if (RandomUtils.getRandomValue(100) < probabilityOfMutation(pop.fitness(individual))*100){
					individual.mutate();
					if (RandomUtils.getRandomValue(100) < 30){
						individual.mutate();
						if (RandomUtils.getRandomValue(100) < 15){
							individual.mutate();
						}
					}
				//	System.out.println("Added new mutation event, for mutated");
					queue.add(new Event(Event.MUTATION, RandomUtils.getRandomTime(calculateMutation(pop.fitness(individual))), individual));
				}
				else {
					//System.out.println("Added new mutation event, for not mutated");
					queue.add(new Event(Event.MUTATION, RandomUtils.getRandomTime(calculateMutationNoMutation(pop.fitness(individual))), individual));
				}
				
			}
			else if (type == Event.REPRODUCTION){
				eventCount = eventCount+1;
				Individual child = individual.reproduce();
				pop.add(child);
				queue.add(new Event(Event.REPRODUCTION, RandomUtils.getRandomTime(calculateReproduction(pop.fitness(child))), child));
				queue.add(new Event(Event.MUTATION, RandomUtils.getRandomTime(calculateMutation(pop.fitness(child))), child));
				queue.add(new Event(Event.DEATH, RandomUtils.getRandomTime(calculateDeath(pop.fitness(child))), child));
			}
				
				
			
			if (pop.size() > maxPopulation){
				pop.epidemic();
				//System.out.println("The purge");
			}	
			
				//System.out.println("Printer \t " + individual.path() + "\t" + type);
			if (loggingType == VERBOSE) {
				System.out.println("Simulating: " + queue.next());
			}
		}
		
		System.out.printf("The best path is: ");
		City[] bestPath = pop.bestPath();
		for (City city : bestPath){
			System.out.printf(city.name() + "; ");

		}
		//System.out.println( "\n");
		double cost = 0;
		for (int i = 1; i<=bestPath.length;i++){
			if (!(i == bestPath.length)){
				//System.out.println("calculating distance from: " + (i-1) +" to " + i);
				cost += bestPath[i].distanceTo(bestPath[i-1]);
			}
			else {
				//System.out.println("calculating distance from: " + (i-1) +" to " + 0);
				cost += bestPath[0].distanceTo(bestPath[i-1]);
			}	
		}
		System.out.println("(cost: " + cost + ")");
		System.out.println("Antal events simulereret: " + eventCount);				
	}

	public static double calculateReproduction(double fitness){
		return (1-Math.log(fitness)*(currentPopulation/maxPopulation))*reproductionInterval;
	}

	public static double calculateMutation(double fitness){
		//System.out.println("fitness: " + fitness + " mutationsInterval: " + mutationsInterval);
		return (1-Math.log(fitness))*mutationsInterval;
	}

	public static double calculateDeath(double fitness){
		return (1-Math.log(1-fitness))*deathInterval;
	}


	public static double probabilityOfDeath(double fitness){
		return 1-Math.pow(fitness,2);
	}

	public static double probabilityOfMutation(double fitness){
		return Math.pow(1-fitness,2);
	} 
	
	public static double calculateMutationNoMutation(double fitness){
		return (1-Math.log(fitness))*mutationsInterval/10;
	} 
	


}
	



	


	
