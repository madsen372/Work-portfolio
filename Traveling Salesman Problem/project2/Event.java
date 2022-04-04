
public class Event {

	public static final char DEATH = 'd';
	public static final char MUTATION = 'm';
	public static final char REPRODUCTION = 'r';

	private char eventType;
	private double timeStamp;
	private Individual individual;
	
	
	public Event(char eventType, double timeStamp, Individual individual) {
		this.eventType = eventType;
		this.timeStamp = timeStamp;
		this.individual = individual;

	}
	
	public char type() {
		return this.eventType;

	}

	public double time() {
		return this.timeStamp;
	}

	public Individual individual() {
		return this.individual;
	}

	public String toString() {
		String event1 = "Event type: " + DEATH + " time: " + this.timeStamp;
		String event2 = "Event type: " + MUTATION + " time: " + this.timeStamp;
		String event3 = "Event type: " + REPRODUCTION + " time: " + this.timeStamp;
		
		if((this.eventType == DEATH)){
			return event1;
		} 
		else if((this.eventType == MUTATION)){
			return event2;

		} 
		else {
			return event3;
		}
		
		
	}
	






















}