package tech.ojizero.randomquiz.data_classes;

/**
 * Created by oji on 3/9/17.
 */

public class Product {
	private int
		id,
		price;
	private String
		name;

	public Product (int id, int price, String name) {
		this.id = id;
		this.price = price;
		this.name = name;
	}

	public Product (int price, String name) {
		this.price = price;
		this.name = name;
	}

	public int getId () {
		return id;
	}

	public void setId (int id) {
		this.id = id;
	}

	public int getPrice () {
		return price;
	}

	public void setPrice (int price) {
		this.price = price;
	}

	public String getName () {
		return name;
	}

	public void setName (String name) {
		this.name = name;
	}

	@Override
	public String toString () {
		return String.format("ID:%s,NAME:%s,PRICE:%s", this.id, this.name, this.price);
	}
}
