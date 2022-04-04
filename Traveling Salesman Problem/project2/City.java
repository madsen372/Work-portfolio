import java.lang.Math;

public class City {

	private String name;
	private double x;
	private double y;


	public City(String name, double x, double y) {

		this.name = name;
		this.x = x;
		this.y = y;

		//System.out.println("Created city: " + this.name); Brugte vi til at se, om byerne rent faktisk blev kreeret.

	}

	public String name() {
		return this.name;

	}

	public double x() {
		return this.x;

	}

	public double y() {
		return this.y;

	}

	public double distanceTo(City ny){
		double nyY = Math.abs(this.y - ny.y);
		double nyX = Math.abs(this.x - ny.x);
		double distance = Math.sqrt((nyX*nyX)+(nyY*nyY));
		System.out.println(distance);
		return distance;
	}
}

