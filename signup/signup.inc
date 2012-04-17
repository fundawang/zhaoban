<?php

/*
 * This file is licensed under GPLv2+.
 */

define('signup_user_PROFESSION', 'profession');
define('signup_user_EXAM', 'exam');
define('signup_user_CONTACT', 'contact');
define('signup_user_GAOKAO', 'gaokao');

define('signup_progress_PERSONALINFORMATION', 0);
define('signup_progress_PROFESSION', 1);
define('signup_progress_EXAM', 2);
define('signup_progress_CONTACT', 3);
define('signup_progress_GAOKAO', 4);
define('signup_progress_FINISH', 10);

define('signup_block_PROGRESS', 'signupprogress');
define('signup_block_HOTPROFESSIONS', 'hotprofessions');
define('signup_block_SEARCHSTUDENTS', 'searchstudents');


define('uc_OWN_STUDENT',0);
define('uc_MAINLAND_STUDENT',1);
define('uc_HKTW_STUDENT',2);
define('uc_OVERSEA_STUDENT',3);

function _signup_determine_period($user) {
        // Determine whether you have entered your personal information.
        $result = db_select('students', 's')->fields('s')->condition('s.uid',$user->name)->execute()->fetchObject();
        if(!($result->idcard_number) || !($result->name))
                return signup_progress_PERSONALINFORMATION; // One must fill in personal information at first.

        // Determine whether you have signed up professions
		$result = db_select('students', 's')->fields('s',array('profession_major', 'examplace'))->condition('s.uid',$user->name)->execute()->fetchObject();
        if($result->profession_major == '0' || $result->examplace=='' )
                return signup_progress_PROFESSION; // One must fill in profession at first.
        
        // Determine whether you have selected the professor
        $result = db_select('vl_teachers','t')->fields('t')->condition('t.major_profession',$result->profession_major)->countQuery()->execute()->fetchField();
        if(($result>0) && (db_select('students','s')->fields('s',array('chosen_professor'))->condition('s.uid',$user->name)->execute()->fetchField()==''))
                return signup_progress_PROFESSION; // One must select his profesor.

        // Determine whether you have filled up exams
        $query = db_select('students_going','sg')->condition('sg.uid',$user->name);
	$query->innerJoin('vl_exam','e','sg.eid=e.eid');
        $query->addExpression('count(*)');
	$query=$query->condition('e.opus','0','>')->isNull('sg.opus');
        $result=$query->execute()->fetchField();
        if($result)
                return signup_progress_EXAM; // One must fill in exams at first.

        // Determine whether you have filled up your contact information
        //$result = db_result(db_query('SELECT count(*) FROM {students_contact} WHERE uid=\'%s\'', $user->name));
        $result = db_select('students_contact','sc')->fields('sc',array('contact_phone'))->condition('sc.uid',$user->name)->countQuery()->execute()->fetchField();
        if(!$result)
                return signup_progress_CONTACT; // One must fill in contact at first.

        // All will be fine if our own student or foreign student
        if($user->ucategory == 0 || $user->ucategory == 3) return signup_progress_FINISH;

        // Determine whether you hvae filled up your gaokao informatino
        $result=db_select('students_gk','sg')->fields('sg')->condition('sg.uid',$user->name)->execute()->fetchObject();
        if(!isset($result->city_district) || ($result->city_district == $result->province && $user->ucategory == 1))
                return signup_progress_GAOKAO; // One must fill his city_district

        return signup_progress_FINISH;
}

/**
 * Function that computes dateofbirth from id number
 */
function pim_dobfromidnumber($idnumber)
{
  if(strlen($idnumber) == 18)
    return substr($idnumber,6,8);
  else if(strlen($idnumber) == 15)
    return '19'.substr($idnumber,6,6);
  else return '20000101';
}

 /**
  * Function that computes gender from id number
  */
 function pim_genderfromidnumber($idnumber)
 {
   if(strlen($idnumber) == 18)
         return 2-(substr($idnumber,16,1) % 2);
   else if(strlen($idnumber) == 15)
         return 2-(substr($idnumber,14,1) % 2);
   else return 0;
 }

 /**
  * Function that determine whether YYYYMMDD is a valid date
  */
 function is_date_validate($datestr) {
        switch (strlen($datestr)) {
                case 6:
                        $datestr = '19'.$datestr;
                case 8:
                        $year = substr($datestr,0,4);
                    $month = substr($datestr,4,2);
                    $day = substr($datestr,6,2);
                    return checkdate($month, $day, $year);
                default: return false;
        }
}

 /**
  * Function that computes the capthca char of IDCard number.
  */
function idc_get_captcha($idcardn) {
        $idcardn = substr($idcardn, 0, 17);
        $sum = 0;
        $weight = array (7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2);
        for ($i=0; $i<17; $i++) {
                $sum += substr($idcardn, $i, 1)* $weight[$i];
        }
        $captcha = 12 - ($sum % 11);
        switch ($captcha) {
                case 10: $captcha = 'X'; break;
                case 11: $captcha = '0'; break;
                case 12: $captcha = '1'; break;
                default: $captcha = sprintf('%d',$captcha);
        }
        return $captcha;
}

function is_idcardn_validate($idcardn) {
        switch (strlen($idcardn)) {
                case 15:
                    if (preg_match('/\d{15}/', $idcardn) == 0)
                        return false;
                    if (!is_date_validate(substr($idcardn,6,6)))
                        return false;
                    return true;
                case 18:
                    if (preg_match('/\d{17}[\dX]/', $idcardn) == 0)
                        return false;
                    if (!is_date_validate(substr($idcardn,6,8)))
                        return false;
                    if (idc_get_captcha($idcardn) != substr($idcardn, 17, 1))
                        return false;
                    return true;
                default: return false;
        }
}

/*
 * Function that validate if a Hong Kong idcarnumber is valid
 * Based on following link:
 * http://zhidao.baidu.com/question/278277.html
 */
function is_hk_idcardn_validate($idcardn) {
        if (strlen($idcardn) != 10)
          return false;
        if (substr($idcardn,7,1) != '(') return false;
        if (substr($idcardn,9,1) != ')') return false;
        for($i=0,$s=0;$i<7;$i++)
        {
                $val = substr($idcardn,$i,1);
                if(!is_numeric($val)) $val = ord($val) - ord('A') + 1;
                $s += $val * (8-$i);
        }
        $s = (int)$s % 11;
        $captcha=substr($idcardn,8,1);
        if($s == 0)
                if($captcha == '0') return true;
            else return false;
        else if($s == 1) {
                if($captcha == 'A') return true;
            else return false;
        }
        else if($captcha == 11-$s) return true;
        else return false;
}

/*
 * Function that validate if a Taiwan idcarnumber is valid
 * Based on the following link
 * http://zhidao.baidu.com/question/1202303.html
 */
function is_tw_idcardn_validate($idcardn) {
        // Conversion table for the 1st character.
        $code_alphabet = array(
                'A' => 1,
                'B' => 10,
                'C' => 19,
                'D' => 28,
                'E' => 37,
                'F' => 46,
                'G' => 55,
                'H' => 64,
                'I' => 39,
                'J' => 73,
                'K' => 82,
                'L' => 2,
                'M' => 11,
                'N' => 20,
                'O' => 48,
                'P' => 29,
                'Q' => 38,
                'R' => 47,
                'S' => 56,
                'T' => 65,
                'U' => 74,
                'V' => 83,
                'W' => 3,
                'X' => 12,
                'Y' => 21,
                'Z' => 30);
        if (strlen($idcardn)!=10) return false;
        if (substr($idcardn,1,1) !== '1' && substr($idcardn,1,1) !== '2') return false;
        if (preg_match('/[A-Z]\d{9}/', $idcardn) == 0) return false;
        else return true;
}

function _is_valid_remote_address() {
	if((strpos($_SERVER['REMOTE_ADDR'], '10.1.0'))===FALSE) return false;
	else return true;
}
?>
