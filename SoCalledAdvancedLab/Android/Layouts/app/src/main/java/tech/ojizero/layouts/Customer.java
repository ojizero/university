package tech.ojizero.layouts;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

/**
 * Created by oji on 2/23/17.
 */

enum Gender {
	Male, Female, Undefined
}

class Customer {
	public static ArrayList<Customer> List    = new ArrayList<>();
	public static java.util.List      Genders = (List) new ArrayList<>(Arrays.asList(new String[]{
		"male", "female", "undefined"
	}));

	public static final int GENDER_MALE      = 0;
	public static final int GENDER_FEMALE    = 1;
	public static final int GENDER_UNDEFINED = 2;

	private int    id;
	private String name, phone;
	private String gender;

	public Customer (int id, String name, String phone, String gender, Date birth) {
		this.id = id;
		this.name = name;
		this.phone = phone;
		this.gender = gender;
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

	public String getGender () {
		return gender;
	}

	public void setGender (String gender) {
		this.gender = gender;
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
			);
		} else {
			return false;
		}
	}
}