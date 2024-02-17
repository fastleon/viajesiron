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

}