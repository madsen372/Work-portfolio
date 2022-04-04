
public class Individual {

	private City[] cities;
	

	//laver selve Individual constructeren
	public Individual(City[] cities) {
		this.cities = randomizedCities(cities);

		/*for (int i = 0; i<cities.length; i++){
		    System.out.println("Current city: "+cities[i].name());
    	} 

    	for-loop vi brugte til at teste, om vi kunne se current cities.*/
	}	


	public City[] path() {
		return this.cities;
	}

	public double cost() {
		return costMethod(this.cities);
	}


	private static City[] randomizedCities(City[] cities) {
		City[] randomizedCities = new City[cities.length];  // New array for randomized cities [null, null, .., null]

		for(int i = 0; i<randomizedCities.length; i++) {  // Loop through cities
			int randomCity = RandomUtils.getRandomValue(randomizedCities.length); //random position for a city in the array
			City currentCity = cities[i];					// Get current city
			randomizedCities[i] = cities[randomCity];
			
		}
		return randomizedCities;

	}

	//Distance fra by til by.
	private static double costMethod(City[] cities) {
		double costToTravel = 0;
    	for (int i = 1; i < cities.length; i++){
			costToTravel+=cities[i-1].distanceTo(cities[i]);
    }
    	return costToTravel;
	}

	//Får random value fra byerne og bytter rundt på dem
	public void mutate(){
		int i = RandomUtils.getRandomValue(this.cities.length);
		int j = RandomUtils.getRandomValue(this.cities.length);

		City swap = this.cities[i];
		this.cities[i] = this.cities[j];
		this.cities[j] = swap;

	}

	//Benytter vores mutate metode til at kreere et nyt individual, fordi reproduction skal "Return a muted copy of this individual"
	public Individual reproduce() {
		Individual individual = new Individual(this.cities);
		individual.mutate();
		return individual;
	}


}


