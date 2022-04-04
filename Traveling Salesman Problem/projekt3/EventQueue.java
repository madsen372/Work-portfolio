import java.util.ArrayList;
import java.util.Iterator;



public class EventQueue {
	
	private ArrayList eventq;

	public EventQueue() {
		eventq = new ArrayList();

	}

	public void add(Event e) {
		eventq.add(e);
	}


	public Event next() {
		
		Event event = (Event)eventq.get(0); //mby, for at den starter med den første, så ved sgu ik

		Iterator iterator = eventq.iterator();
			
			while(iterator.hasNext()) {
				if(iterator.next() == event){
				iterator.remove();
				}

			}

		return event;
	}


/*
		Event event = (Event)eventq.get(0);
		for (Object object : eventq)
			object = event;
		eventq.remove(event);
		return event;

	}
		
	*/	

	public boolean hasNext()  {
     try
   	 {
      eventq.get(0);
      return true;
    } catch (IndexOutOfBoundsException localIndexOutOfBoundsException) {}
    return false;
  }
  

}


