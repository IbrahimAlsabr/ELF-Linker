#!/bin/bash


# verification header
function ver_header()
{	./main $1 -h > resultat_projet_p.txt
	arm-none-eabi-readelf -h $1 > resultat_commande_p.txt
	
	# afficher les resultats sans la ligne indiquant la machine
	grep -v "Machine:" resultat_projet_p.txt > resultat_projet_p2.txt
	grep -v "Machine:" resultat_commande_p.txt > resultat_commande_p2.txt

    grep -v "OS/ABI:" resultat_projet_p2.txt > resultat_projet.txt
	grep -v "OS/ABI:" resultat_commande_p2.txt > resultat_commande.txt

    #comparaison et redirection du resultat
	diff resultat_projet.txt resultat_commande.txt 
	if [[ $? -eq 0 ]]
	then
		echo "headers identiques"
	else
		echo "headers differents"
	fi
	echo ""
}


# verification de la table des sections
function ver_sect_table()
{	./main $1 -S > resultat_projet.txt
	arm-none-eabi-readelf -S $1 > resultat_commande.txt
	
	diff resultat_projet.txt resultat_commande.txt 
	if [[ $? -eq 0 ]]
	then
		echo "tables section identiques"
	else
		echo "tables section differents"
	fi
}

# verification de la table des symboles
function ver_symb_table()
{	./main $1 -s > resultat_projet.txt
	readelf -s $1 > resultat_commande.txt
	
	diff resultat_projet.txt resultat_commande.txt > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "tables symbole identique"
	else
		echo "tables symbole differentes"
	fi
	echo ""
}


# verification de la table de reimplentation
function ver_reimp_table()
{	./main $1 -r > resultat_projet.txt
	readelf -r $1 > resultat_commande.txt
	
	diff resultat_projet.txt resultat_commande.txt > /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "tables reimplentation identique"
	else
		echo "tables reimplentation differentes"
	fi
	echo ""
}



function tests()
{	if [[ -e $1 ]]
	then
		ver_header $1
		ver_sect_table $1
		ver_symb_table $1
		ver_reimp_table $1
		rm resultat_projet.txt resultat_commande.txt resultat_projet_p.txt resultat_commande_p.txt resultat_projet_p2.txt resultat_commande_p2.txt
	else
		echo "Error: File '$1' not find"
	fi
}

tests $1
