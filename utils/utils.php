<?php

class Utils {

    public static function toArray($objeto) {
        $resultado = array();
        $reflector = new ReflectionClass($objeto);
        $propiedades = $reflector->getProperties(ReflectionProperty::IS_PRIVATE);
        foreach($propiedades as $propiedad) {
            $propiedad->setAccessible(true);
            $resultado[$propiedad->getName()] = $propiedad->getValue($objeto);
            $propiedad->setAccessible(false);
        }
        return $resultado;
    }

    public static function dateToDMY($date) {
        if (!empty($date)) {
            return ( date("d-m-Y", strtotime($date)) );
        } else {
            return false;
        }
    }

    public static function dateToYMD($date) {
        if (!empty($date)) {
            return ( date("Y-m-d", strtotime($date)) );
        } else {
            return false;
        }
    }

}