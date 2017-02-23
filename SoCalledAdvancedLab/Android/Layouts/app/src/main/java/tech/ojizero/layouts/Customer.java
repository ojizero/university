package tech.ojizero.layouts;

import java.util.ArrayList;
import java.util.Date;

/**
 * Created by oji on 2/23/17.
 */

enum Gender {
	Male, Female, Undefined
}

class Customer {
	public static ArrayList<Customer> list = new ArrayList<>();

	private int    id;
	private String name, phone;
	private Gender gender;
	private Date   birth;

	public Customer (int id, String name, String phone, Gender gender, Date birth) {
		this.id = id;
		this.name = name;
		this.phone = phone;
		this.gender = gender;
		this.birth = birth;
	}

	public int getId () {
		return id;
	}

	public void setId (int id) {
		this.id = id;
	}

	/**
	 * Setters and getters
	 **/
	public String getName () {
		return name;
	}

	public void setName (String name) {
		this.name = name;
	}

	public String getPhone () {
		return phone;
	}

	public void setPhone (String phone) {
		this.phone = phone;
	}

	public Gender getGender () {
		return gender;
	}

	public void setGender (Gender gender) {
		this.gender = gender;
	}

	public Date getBirth () {
		return birth;
	}

	public void setBirth (Date birth) {
		this.birth = birth;
	}

	@Override
	public String toString () {
		return String.format("%s - %s", this.id, this.name);
	}

	@Override
	public boolean equals (Object obj) {
		if (obj instanceof Customer) {
			Customer o = (Customer) obj;
			return (
				this.id == o.getId()
				&&
				this.name.equals(o.name)
				&&
				this.phone.equals(o.getPhone())
				&&
				this.birth.compareTo(o.getBirth()) == 0
			);
		} else {
			return false;
		}
	}
}