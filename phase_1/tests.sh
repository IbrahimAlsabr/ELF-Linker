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

	echo -e "\033[31m ENTETE \033[0m"
	diff -b resultat_projet.txt resultat_commande.txt 
	if [[ $? -eq 0 ]]
	then
		echo -e "\033[32m headers identiques \033[0m"
	fi
	echo ""
}


# verification de la table des sections
function ver_sect_table()
{	./main $1 -S > resultat_projet.txt
	arm-none-eabi-readelf -S $1 > resultat_commande.txt
	
	echo -e "\033[31m TABLE SECTIONS \033[0m"
	diff -b resultat_projet.txt resultat_commande.txt 
	if [[ $? -eq 0 ]]
	then
		echo -e "\033[32m table sections identique \033[0m"
	fi
}

# verification de la table des symboles
function ver_symb_table()
{	./main $1 -s > resultat_projet.txt
	arm-none-eabi-readelf -s $1 > resultat_commande.txt
	echo -e "\033[31m TABLE SYMBOLES \033[0m"
	diff -b resultat_projet.txt resultat_commande.txt 
	if [[ $? -eq 0 ]]
	then
		echo -e "\033[32m table symboles identique \033[0m"
	fi
	echo ""
}


# verification de la table de reimplentation
function ver_reimp_table()
{	./main $1 -r > resultat_projet_p.txt
	arm-none-eabi-readelf -r $1 > resultat_commande_p.txt
	
	grep -v "section" resultat_projet_p.txt > resultat_projet_p2.txt
	grep -v "section" resultat_commande_p.txt > resultat_commande_p2.txt

	grep -v "Offset" resultat_projet_p2.txt > resultat_projet_p3.txt
	grep -v "Offset" resultat_commande_p2.txt > resultat_commande_p3.txt

	cut -d ' ' -f 1-4 resultat_commande_p3.txt > resultat_commande_p4.txt

	echo -e "\033[31m TABLES REIMPLANTATION \033[0m"
	diff -b resultat_projet_p3.txt resultat_commande_p4.txt 
	if [[ $? -eq 0 ]]
	then
		echo -e "\033[32m tables reimplentation identique \033[0m"
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
